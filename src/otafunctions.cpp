#include "otafunctions.h"
#include "config.h"
#include "logging.h"
#include "Adafruit_NeoPixel.h"
#include <ArduinoOTA.h>
#include <LittleFS.h>

extern Adafruit_NeoPixel *g_LED_STRIPS;

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
        LittleFS.end();

        LOG(Log_Info, "Start updating " + type);
        for (int i = 0; i < NUM_STRIPS; i++) {
            Adafruit_NeoPixel *strip = &g_LED_STRIPS[i];
            strip->clear();
            strip->show();
        }
    });

    ArduinoOTA.onEnd([]() {
        for (int i = 0; i < NUM_STRIPS; i++) {
            Adafruit_NeoPixel *strip = &g_LED_STRIPS[i];
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
        LOG(Log_Info, String("Progress:") + (progress / (total / 100)));

        for (int i = 0; i < NUM_STRIPS; i++) {
            Adafruit_NeoPixel *strip = &g_LED_STRIPS[i];
            strip->setBrightness(LED_BASE_BRIGHTNESS);
            int px = LEDS_PER_STRIP - ((LEDS_PER_STRIP * progress) / total);
            strip->setPixelColor(px, strip->Color(0x00, 0xFF, 0x00));
            strip->show();
        }
    });

    ArduinoOTA.onError([](ota_error_t error) {
        LOG(Log_Error, "Error: ");
        LOG(Log_Error, error);
        if (error == OTA_AUTH_ERROR)
            LOG(Log_Error, "Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            LOG(Log_Error, "Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            LOG(Log_Error, "Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            LOG(Log_Error, "Receive Failed");
        else if (error == OTA_END_ERROR)
            for (int i = 0; i < NUM_STRIPS; i++)
            {
                Adafruit_NeoPixel *strip = &g_LED_STRIPS[i];
                strip->clear();
                strip->setBrightness(LED_BASE_BRIGHTNESS);
                for (int i = 0; i < LEDS_PER_STRIP; i++)
                {
                    strip->setPixelColor(i, strip->Color(0xFF, 0x00, 0x00));
                }
                strip->show();
            }
        LOG(Log_Error, "End Failed");
    });

    ArduinoOTA.begin();
}

