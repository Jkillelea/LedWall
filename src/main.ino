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
// DropRenderer<NUM_STRIPS, LEDS_PER_STRIP> dropRenderer;
// Renderer<NUM_STRIPS, LEDS_PER_STRIP>     *g_LedRenderer;
// DropRenderer<NUM_STRIPS, LEDS_PER_STRIP> *g_LedRenderer;
std::shared_ptr<Renderer<NUM_STRIPS, LEDS_PER_STRIP>> g_LedRenderer;

ServerActions_t serverActions[] = 
{
    {"/",    handleRoot},
    {"/hue", handleHue}
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
            LOG(Log_Info, dir.fileName() + " :size: " + dir.fileSize());
        }
        else
        {
            LOG(Log_Info, dir.fileName());
            listFiles(dir.fileName());
        }
    }
}

void setup()
{
    LOGGER_BEGIN;

    LOG(Log_Trace, "Init strips");
    for (Adafruit_NeoPixel &strip : g_LED_STRIPS)
    {
        strip.begin();
        strip.setBrightness(LED_BASE_BRIGHTNESS); // 0 - 255
        strip.clear();
        strip.show();
    }

    g_LedRenderer =
        std::shared_ptr<DropRenderer<NUM_STRIPS, LEDS_PER_STRIP>>(new DropRenderer<NUM_STRIPS, LEDS_PER_STRIP>());

    LOG(Log_Trace, "FS init");
    if (!LittleFS.begin())
    {
        LOG(Log_Error, "Failed to start FS!");
    }
    else
    {
        FSInfo info;
        if (LittleFS.info(info))
        {
            LOG(Log_Info, String("totalBytes:    ") + info.totalBytes);
            LOG(Log_Info, String("usedBytes:     ") + info.usedBytes);
            LOG(Log_Info, String("blockSize:     ") + info.blockSize);
            LOG(Log_Info, String("pageSize:      ") + info.pageSize);
            LOG(Log_Info, String("maxOpenFiles:  ") + info.maxOpenFiles);
            LOG(Log_Info, String("maxPathLength: ") + info.maxPathLength);
        }
        listFiles("/");
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    LOG(Log_Debug, "Waiting on wifi...");
    while (!WiFi.isConnected())
    {
        delay(100);
    }

    LOG(Log_Warning, String("Connected to ") + WiFi.SSID());
    LOG(Log_Warning, String("IP ") + WiFi.localIP().toString());

    LOG(Log_Trace, "OTA init");
    otaInit();

    LOG(Log_Trace, "Registering webserver callbacks");
    g_WebServer.onNotFound(handleNotFound);
    for (ServerActions_t &action : serverActions)
    {
        LOG(Log_Trace,action.path);
        g_WebServer.on(action.path, action.callback);
    }
    g_WebServer.begin();

    LOG(Log_Debug, "Setup done");
}

void loop()
{
    LOG(Log_Trace, "Main loop");

    /* Physics calculations */
    g_LedRenderer->render();

    /* Write to LEDs */
    showLeds();

    /* Upload server handling */
    ArduinoOTA.handle();

    /* Web server handling */
    g_WebServer.handleClient();

    /* Wait */
    // delay(1);
}

