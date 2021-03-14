#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <ESP8266WiFi.h>
#include "config.h"
#include "drop.h"
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

void showLeds(void)
{
    for (Adafruit_NeoPixel &strip : g_LED_STRIPS) {
        strip.show();
    }
}

int dropYIdktoLED(int ypos)
{
    // If the drop is still off the top of the screen return -1
    if (ypos > 0)
    {
        return -1;
    }
    else
    {
        return abs(ypos);
    }
}

void renderDrops(void)
{
    for (Drop &d : g_DROPS)
    {
        d.run(); // do physics
        int strip_num = (int) d.xPosition();
        int ypixel    = dropYIdktoLED((int) d.yPosition());
        int yspeed    = abs((int) d.yVelocity());


        if (ypixel >= 0 && strip_num < NUM_STRIPS)
        {
            LOG(Log_Trace, String("strip: ") + strip_num);
            LOG(Log_Trace, String("ypos: ") + d.yPosition());
            LOG(Log_Trace, String("ypixel: ") + ypixel);

            Adafruit_NeoPixel *strip = &g_LED_STRIPS[strip_num];
            strip->setPixelColor(ypixel, strip->Color(d.color(0), d.color(1), d.color(2)));

            for (int tail = 0; tail < ypixel; tail++)
            {
                int distance = ypixel - tail;
                LOG(Log_Info, String("distance: ") + distance);
                uint32_t color = strip->Color(d.color(0) * yspeed / (distance * distance),
                                              d.color(1) * yspeed / (distance * distance),
                                              d.color(2) * yspeed / (distance * distance));

                if (distance < LEDS_PER_STRIP)
                {
                    strip->setPixelColor(tail, color);
                }
                else
                {
                    strip->setPixelColor(tail, 0);
                }
            }

            // drop goes off bottom
            if (ypixel > 2 * LEDS_PER_STRIP)
            {
                LOG(Log_Trace, String("Resetting drop on strip ") + strip_num);

                strip->clear();

                d = Drop((double) random(0, NUM_STRIPS),
                         (double) random(0, 10 * LEDS_PER_STRIP)
                         );
            }
        }
    }
}

void setup()
{
    LOGGER_BEGIN;

    for (Adafruit_NeoPixel &strip : g_LED_STRIPS)
    {
        strip.begin();
        strip.setBrightness(LED_BASE_BRIGHTNESS); // 0 - 255
        strip.clear();
        strip.show();
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    LOG(Log_Debug, "Waiting on wifi...");
    while (!WiFi.isConnected())
    {
        delay(100);
    }

    LOG(Log_Info, String("Connected to ") + WiFi.SSID());
    LOG(Log_Info, String("IP ") + WiFi.localIP().toString());

    LOG(Log_Trace, "OTA init");
    otaInit();

    LOG(Log_Trace, "Generate drops");
    for (Drop &d : g_DROPS)
    {
        d = Drop(random(0, MAX_DROPS), random(0, LEDS_PER_STRIP / 3));
    }
    LOG(Log_Trace, "done");

    LOG(Log_Debug, "Setup done");
}

static uint64_t lasttimestamp = 0;
void loop() {
    LOG(Log_Trace, "Main loop");

    /* Physics calculations */
    renderDrops();

    /* Write to LEDs */
    showLeds();

    /* Upload server handling */
    ArduinoOTA.handle();

    /* Wait */
    delay(15);
}

