#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "config.h"
#include "drop.h"
#include "droprender.hpp"
#include "otafunctions.h"
#include "logging.h"

// Declare our NeoPixel strip object:
Adafruit_NeoPixel g_LED_STRIPS[] = {
    Adafruit_NeoPixel(LEDS_PER_STRIP, D1, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D2, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D4, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D5, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D6, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D7, NEO_GRB + NEO_KHZ800),
};

#define MAX_DROPS (10)
Drop g_DROPS[MAX_DROPS];

/* HTTP web server */
ESP8266WebServer server(80);

/* Rendering engine */
DropRenderer<NUM_STRIPS, LEDS_PER_STRIP> dropRenderer;

/* server callbacks */
void handleRoot(void)
{
    String text = String("Hello, runtime is ") + millis() + " milliseconds\r\n\r\n";
    server.send(200, "text/plain", text);
}

void handleUptime(void)
{
    String text = String(millis());
    server.send(200, "text/plain", text);
}


void handleNotFound(void)
{
    server.send(404, "text/plain", "not found\r\n\r\n");
    LOG(Log_Error, "Server could not find requested path!");
    switch (server.method())
    {
        case HTTP_ANY:
            LOG(Log_Error, "Method: HTTP_ANY");
            break;
        case HTTP_GET:
            LOG(Log_Error, "Method: HTTP_GET");
            break;
        case HTTP_HEAD:
            LOG(Log_Error, "Method: HTTP_HEAD");
            break;
        case HTTP_POST:
            LOG(Log_Error, "Method: HTTP_POST");
            break;
        case HTTP_PUT:
            LOG(Log_Error, "Method: HTTP_PUT");
            break;
        case HTTP_PATCH:
            LOG(Log_Error, "Method: HTTP_PATCH");
            break;
        case HTTP_DELETE:
            LOG(Log_Error, "Method: HTTP_DELETE");
            break;
        case HTTP_OPTIONS:
            LOG(Log_Error, "Method: HTTP_OPTIONS");
            break;
        default:
            LOG(Log_Error, "Method: UNKNOWN");
            break;
    }

    LOG(Log_Error, "== Args ==");
    for (int i = 0; i < server.args(); i++)
    {
        String line = server.argName(i) + " : " + server.arg(i);
        LOG(Log_Error, line);
    }

    LOG(Log_Error, "== Headers ==");
    for (int i = 0; i < server.headers(); i++)
    {
        String line = server.headerName(i) + " : " + server.header(i);
        LOG(Log_Error, line);
    }
}

void showLeds(void)
{
    for (Adafruit_NeoPixel &strip : g_LED_STRIPS)
    {
        strip.show();
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

    LOG(Log_Trace, "Generate drops");
    for (Drop &d : g_DROPS)
    {
        d = Drop(random(0, MAX_DROPS), random(0, LEDS_PER_STRIP / 3));
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

    LOG(Log_Trace, "Webserver callbacks");
    server.on("/", handleRoot);
    server.on("/uptime", handleUptime);
    server.onNotFound(handleNotFound);
    server.begin();

    LOG(Log_Debug, "Setup done");
}

void loop() {
    LOG(Log_Trace, "Main loop");

    /* Physics calculations */
    dropRenderer.render();

    /* Write to LEDs */
    showLeds();

    /* Upload server handling */
    ArduinoOTA.handle();

    /* Web server handling */
    server.handleClient();

    /* Wait */
    delay(1);
}

