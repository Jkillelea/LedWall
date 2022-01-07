#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <cstdio>
#include <math.h>
#include <memory>
#include <stdint.h>
#include <string.h>

#include "ESP8266WiFiGeneric.h"
#include "config.h"
#include "drop.h"
#include "renderer.hpp"
#include "droprenderer.hpp"
#include "globals.hpp"
#include "logging.h"
#include "otafunctions.h"
#include "pixelrenderer.hpp"
#include "serveractions.hpp"
#include "whitewallrenderer.hpp"
#include "rollingrainbowrenderer.hpp"

// Declare our NeoPixel strip object:
Adafruit_NeoPixel g_LED_STRIPS[] =
{
    Adafruit_NeoPixel(LEDS_PER_STRIP, D1, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D2, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D4, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D5, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D6, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D7, NEO_GRB + NEO_KHZ800),
};

/* HTTP web server */
ESP8266WebServer g_WebServer(80);

/* Rendering engine */
std::shared_ptr<Renderer<NUM_STRIPS, LEDS_PER_STRIP>> g_LedRenderer;

/* WiFi Callback handlers */
WiFiEventHandler onStationConnectedHandler;

ServerActions_t serverActions[] = 
{
    {"/",                  handleRoot},
    {"/hue",               handleHue},
    {"/sat",               handleSat},
    {"/brightness",        handleBrightness},
    {"/droprenderer",      handleDropRenderer},
    {"/whitewallrenderer", handleWhitewallRenderer},
    {"/rainbowrenderer",   handleRainbowRenderer},
};


void showLeds(void)
{
    for (Adafruit_NeoPixel &strip : g_LED_STRIPS)
    {
        strip.show();
    }
}

void listFiles(String path)
{
    Dir dir = LittleFS.openDir(path);
    while (dir.next())
    {
        if (dir.isFile())
        {
            LOG_INFO(dir.fileName() + " :size: " + dir.fileSize());
        }
        else
        {
            LOG_INFO(dir.fileName());
            listFiles(dir.fileName());
        }
    }
}

void onStationConnected(const WiFiEventStationModeConnected& evt)
{
    // LOG_WARN(String("WiFi connected to ") + evt.ssid + ", channel " + evt.channel);
    LOG_WARN(String("Connected to ") + WiFi.SSID());
    LOG_WARN(String("IP ") + WiFi.localIP().toString());

    LOG_TRACE("OTA init");
    otaInit();

    LOG_DEBUG("Registering webserver callbacks");
    g_WebServer.onNotFound(handleNotFound);
    for (ServerActions_t &action : serverActions)
    {
        LOG_DEBUG(action.path);
        g_WebServer.on(action.path, action.callback);
    }
    g_WebServer.begin();

    LOG_DEBUG("Setup done");
}


void setup()
{
    LOGGER_BEGIN;
    LOG_INFO("Boot");
    delay(100);

    LOG_TRACE("Init strips");
    for (Adafruit_NeoPixel &strip : g_LED_STRIPS)
    {
        strip.begin();
        strip.setBrightness(LED_BASE_BRIGHTNESS); // 0 - 255
        strip.clear();
        strip.show();
    }

    // g_LedRenderer = std::shared_ptr<DropRenderer<NUM_STRIPS, LEDS_PER_STRIP>>(
    //         new DropRenderer<NUM_STRIPS, LEDS_PER_STRIP>());
    g_LedRenderer = std::shared_ptr<Renderer<NUM_STRIPS, LEDS_PER_STRIP>>(new RollingRainbowRenderer<NUM_STRIPS, LEDS_PER_STRIP>());

    LOG_TRACE("FS init");
    if (!LittleFS.begin())
    {
        LOG_ERROR("Failed to start FS!");
    }
    else
    {
        FSInfo info;
        if (LittleFS.info(info))
        {
            LOG_INFO(String("totalBytes:    ").concat(info.totalBytes));
            LOG_INFO(String("usedBytes:     ").concat(info.usedBytes));
            LOG_INFO(String("blockSize:     ").concat(info.blockSize));
            LOG_INFO(String("pageSize:      ").concat(info.pageSize));
            LOG_INFO(String("maxOpenFiles:  ").concat(info.maxOpenFiles));
            LOG_INFO(String("maxPathLength: ").concat(info.maxPathLength));
        }
        listFiles("/");
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    onStationConnectedHandler = WiFi.onStationModeConnected(&onStationConnected);

    // WiFiUDP udp;
    // udp.beginMulticast(IPAddress interfaceAddr, IPAddress multicast, uint16_t port)
}

void loop()
{
    const uint32_t INTERVAL_MS = 15;
    static uint32_t lastMillis = millis();
    uint32_t now   = millis();
    uint32_t delta = 0;
    if (now > lastMillis)
    {
        delta = now - lastMillis;
    }

    LOG_TRACE("Main loop");

    if (delta > INTERVAL_MS)
    {
        lastMillis += INTERVAL_MS + 1;

        /* Physics calculations */
        g_LedRenderer->render();

        /* Write to LEDs */
        showLeds();
    }

    /* Upload server handling */
    ArduinoOTA.handle();

    /* Web server handling */
    g_WebServer.handleClient();

    /* Wait */
    delay(1);
}

