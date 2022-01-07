#ifndef ROLLING_RAINBOW_RENDERER_H_
#define ROLLING_RAINBOW_RENDERER_H_

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "core_esp8266_features.h"
#include "renderer.hpp"
#include "logging.h"
#include <stdint.h>


template<int numStrips, int ledsPerStrip>
class RollingRainbowRenderer : public Renderer<numStrips, ledsPerStrip>
{
    private:
        uint16_t hue_increment = 10;

    public:
        RollingRainbowRenderer() : Renderer<numStrips, ledsPerStrip>()
        {
            this->sat = 0xFF; // Want colorful output
            this->hue_increment = 10; // Max 0xFFFF
        }

        ~RollingRainbowRenderer()
        {
        }

        void setIncrement(uint16_t inc)
        {
            this->hue_increment = inc;
        }

        void render()
        {
            const uint16_t HUE_MAX = 0xFFFF / 2;
            const uint16_t DELTA_HUE = HUE_MAX / NUM_STRIPS;

            for (int s = 0; s < numStrips; s++)
            {
                Adafruit_NeoPixel *strip = this->getStrip(s);
                for (int ledIdx = 0; ledIdx < ledsPerStrip; ledIdx++)
                {
                    uint16_t h = this->hue + (s * DELTA_HUE) + (ledIdx * DELTA_HUE / ledsPerStrip);
                    // strip->setPixelColor(ledIdx, strip->ColorHSV(this->hue + s * DELTA_HUE, this->sat, this->val));
                    strip->setPixelColor(ledIdx, strip->ColorHSV(h, this->sat, this->val));
                }

                this->hue += this->hue_increment;
                LOG_TRACE(this->hue);

                delay(0); // for wifi
            }
        }
};

#endif //ROLLING_RAINBOW_RENDERER_H_
