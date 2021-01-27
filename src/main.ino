#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <cmath>
#include <cstdint>
#include <cstring>
#include "drop.h"

// WiFi
#define WIFI_SSID "HumiditySucks24"
#define WIFI_PASS "freezingpoint!"
// OTA params
#define OTA_HOSTNAME "ledwall" // .local
#define OTA_PORT 8266
// How many NeoPixels are attached to the Arduino?
#define LEDS_PER_STRIP 46

// Declare our NeoPixel strip object:
Adafruit_NeoPixel LED_STRIPS[] = {
    Adafruit_NeoPixel(LEDS_PER_STRIP, D1, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D2, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D4, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D5, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D6, NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(LEDS_PER_STRIP, D7, NEO_GRB + NEO_KHZ800),
};

const int NUM_STRIPS = sizeof(LED_STRIPS) / sizeof(LED_STRIPS[0]);
const int TOTAL_LEDS = NUM_STRIPS * LEDS_PER_STRIP;

#define MAX_DROPS (20)
Drop     drops[MAX_DROPS];
uint32_t LedBuffer[NUM_STRIPS][LEDS_PER_STRIP];


void otaInit() {
    // Port defaults to 8266
    ArduinoOTA.setPort(OTA_PORT);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname(OTA_HOSTNAME);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using
        // SPIFFS.end()

        Serial.println("Start updating " + type);
        for (Adafruit_NeoPixel &strip : LED_STRIPS) {
            strip.clear();
            strip.show();
        }
    });

    ArduinoOTA.onEnd([]() {
        for (Adafruit_NeoPixel &strip : LED_STRIPS) {
            strip.clear();
            strip.setBrightness(10);
            for (int i = 0; i < LEDS_PER_STRIP; i++)
                strip.setPixelColor(i, strip.Color(0x00, 0x00, 0xFF));
            strip.show();
        }
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));

        for (Adafruit_NeoPixel &strip : LED_STRIPS) {
            strip.setBrightness(10);
            int px = LEDS_PER_STRIP - ((LEDS_PER_STRIP * progress) / total);
            strip.setPixelColor(px, strip.Color(0x00, 0xFF, 0x00));
            strip.show();
        }
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)

        for (Adafruit_NeoPixel &strip : LED_STRIPS) {
            strip.clear();
            strip.setBrightness(10);
            for (int i = 0; i < LEDS_PER_STRIP; i++)
                strip.setPixelColor(i, strip.Color(0xFF, 0x00, 0x00));
            strip.show();
        }
        Serial.println("End Failed");
    });

    ArduinoOTA.begin();
}

void setup() {
    Serial.begin(115200);

    bzero(LedBuffer, TOTAL_LEDS);

    for (Drop &d : drops)
    {
        d = Drop(random(0, MAX_DROPS), -random(0, LEDS_PER_STRIP/3));
    }

    for (Adafruit_NeoPixel &strip : LED_STRIPS) {
        strip.begin();
        strip.setBrightness(127); // 0 - 255
        strip.clear();
        strip.show();
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (!WiFi.isConnected()) {
        Serial.println("Waiting on wifi...");
        delay(250);
    }

    Serial.println(String("Connected to ") + WiFi.SSID());
    Serial.println(String("IP ") + WiFi.localIP().toString());

    otaInit();

    for (Adafruit_NeoPixel &strip : LED_STRIPS) {
        strip.clear();
        strip.show();
    }
}


void loop() {
    bzero(LedBuffer, TOTAL_LEDS);

    for (Drop &d : drops)
    {
        d.run(); // do physics
        int xpos = (int) d.xPosition();
        int ypos = (int) fabs(d.yPosition());
        int yvel = (int) fabs(d.yVelocity());

        if ((xpos < 6) && (d.yPosition() <= 0))
        {
            // Adafruit_NeoPixel *strip = &LED_STRIPS[xpos];
            // strip->setPixelColor(ypos, strip->Color(0xFF, 0xFF, 0xFF));

            // for (int tail = 0; tail < ypos; tail++) {
            //     int distance = ypos - tail;
            //     strip->setPixelColor(tail, strip->Color(32 * yvel / (distance*distance),
            //                                             32 * yvel / (distance*distance),
            //                                             32 * yvel / (distance*distance)));
            // }

            uint32_t *strip = LedBuffer[xpos];
            strip[ypos] = (0xFF << 16) | (0xFF << 8) | (0xFF << 0);
            for (int tail = 0; tail < ypos; tail++) {
                int distance = ypos - tail;
                strip[tail] += (32 * yvel / (distance*distance) << 16) |
                               (32 * yvel / (distance*distance) <<  8) |
                               (32 * yvel / (distance*distance) <<  0);
            }
        }

        // drop goes off bottom
        if (ypos > 2*LEDS_PER_STRIP)
        {
            // if (xpos < 6) // clear trail from strip
            //     LED_STRIPS[xpos].clear();

            d = Drop(random(0, NUM_STRIPS), random(0, 10)); // reset drop
        }
    }

    for (int s = 0; s < NUM_STRIPS; s++)
    {
        for (int led = 0; led < LEDS_PER_STRIP; led++)
        {
            LED_STRIPS[s].setPixelColor(led, LedBuffer[s][led]);
        }
    }


    for (Adafruit_NeoPixel &strip : LED_STRIPS)
    {
        strip.show();
    }

    ArduinoOTA.handle();

    // delay(100);
}
