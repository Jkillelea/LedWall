#include "otafunctions.h"
#include "config.h"
#include "Adafruit_NeoPixel.h"
#include <ArduinoOTA.h>

extern Adafruit_NeoPixel *LED_STRIPS;

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

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS
        // using SPIFFS.end()

        Serial.println("Start updating " + type);
        for (int i = 0; i < NUM_STRIPS; i++) {
            Adafruit_NeoPixel *strip = &LED_STRIPS[i];
            strip->clear();
            strip->show();
        }
    });

    ArduinoOTA.onEnd([]() {
        for (int i = 0; i < NUM_STRIPS; i++) {
            Adafruit_NeoPixel *strip = &LED_STRIPS[i];
            strip->clear();
            strip->setBrightness(LED_BASE_BRIGHTNESS);
            for (int i = 0; i < LEDS_PER_STRIP; i++)
            {
                strip->setPixelColor(i, strip->Color(0x00, 0x00, 0xFF)); // blue
            }
            strip->show();
        }
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));

        for (int i = 0; i < NUM_STRIPS; i++) {
            Adafruit_NeoPixel *strip = &LED_STRIPS[i];
            strip->setBrightness(LED_BASE_BRIGHTNESS);
            int px = LEDS_PER_STRIP - ((LEDS_PER_STRIP * progress) / total);
            strip->setPixelColor(px, strip->Color(0x00, 0xFF, 0x00));
            strip->show();
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
            for (int i = 0; i < NUM_STRIPS; i++)
            {
                Adafruit_NeoPixel *strip = &LED_STRIPS[i];
                strip->clear();
                strip->setBrightness(LED_BASE_BRIGHTNESS);
                for (int i = 0; i < LEDS_PER_STRIP; i++)
                {
                    strip->setPixelColor(i, strip->Color(0xFF, 0x00, 0x00));
                }
                strip->show();
            }
        Serial.println("End Failed");
    });

    ArduinoOTA.begin();
}

