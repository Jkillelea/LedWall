#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <ESP8266WiFi.h>
#include "config.h"
#include "drop.h"
#include "otafunctions.h"

// Declare our NeoPixel strip object:
Adafruit_NeoPixel LED_STRIPS[] = {
    Adafruit_NeoPixel(LEDS_PER_STRIP, D1, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D2, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D4, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D5, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D6, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D7, NEO_GRB + NEO_KHZ800),
};

#define MAX_DROPS (50)
Drop drops[MAX_DROPS];

void showLeds(void)
{
    for (Adafruit_NeoPixel &strip : LED_STRIPS) {
        strip.show();
    }
}

void renderDrops(void)
{
    for (Drop &d : drops)
    {
        d.run(); // do physics
        int xpos = (int) d.xPosition();
        int ypos = (int) d.yPosition();
        int yvel = (int) d.yVelocity();

        if ((xpos < NUM_STRIPS) && (ypos <= 0)) {
            Adafruit_NeoPixel *strip = &LED_STRIPS[xpos];
            strip->setPixelColor(abs(ypos), strip->Color(0xFF, 0xFF, 0xFF));

            for (int tail = 0; tail < abs(ypos); tail++) {
                int distance = abs(ypos) - tail;
                uint32_t color = strip->Color(d.color(0) * abs(yvel) / (distance * distance),
                                              d.color(1) * abs(yvel) / (distance * distance),
                                              d.color(2) * abs(yvel) / (distance * distance));

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
            if (ypos < -2 * LEDS_PER_STRIP)
            {
                if (xpos < NUM_STRIPS)
                    strip->clear();
                d = Drop(random(0, NUM_STRIPS), random(0, 2 * LEDS_PER_STRIP)); // reset drop
            }
        }
    }
}

void setup()
{
    Serial.begin(115200);

    for (Drop &d : drops) {
        d = Drop(random(0, MAX_DROPS), -random(0, LEDS_PER_STRIP / 3));
    }

    for (Adafruit_NeoPixel &strip : LED_STRIPS) {
        strip.begin();
        strip.setBrightness(LED_BASE_BRIGHTNESS); // 0 - 255
        strip.clear();
        strip.show();
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (!WiFi.isConnected())
    {
        Serial.println("Waiting on wifi...");
        delay(250);
    }

    Serial.println(String("Connected to ") + WiFi.SSID());
    Serial.println(String("IP ") + WiFi.localIP().toString());

    otaInit();

    for (Adafruit_NeoPixel &strip : LED_STRIPS)
    {
        strip.clear();
        strip.show();
    }
}

void loop() {

    renderDrops();
    showLeds();

    ArduinoOTA.handle();

    // delay(100);
}
