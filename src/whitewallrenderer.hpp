#ifndef WHITEWALL_RENDERER_H_
#define WHITEWALL_RENDERER_H_
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "core_esp8266_features.h"
#include "renderer.hpp"
#include "logging.h"


template<int numStrips, int ledsPerStrip>
class WhiteWallRenderer : public Renderer<numStrips, ledsPerStrip>
{
    public:
        WhiteWallRenderer() : Renderer<numStrips, ledsPerStrip>()
        {
        }

        ~WhiteWallRenderer()
        {
        }

        void render()
        {
            for (int s = 0; s < numStrips; s++)
            {
                Adafruit_NeoPixel *strip = this->getStrip(s);
                for (int ledIdx = 0; ledIdx < ledsPerStrip; ledIdx++)
                {
                    strip->setPixelColor(ledIdx, strip->ColorHSV(this->hue, this->sat, this->val));
                }

                delay(0); // for wifi
            }
        }


    private:
};

#endif // WHITEWALL_RENDERER_H_
