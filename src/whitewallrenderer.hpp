#ifndef WHITEWALL_RENDERER_H_
#define WHITEWALL_RENDERER_H_
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "renderer.hpp"
#include "logging.h"


template<int numStrips, int ledsPerStrip>
class WhiteWallRenderer : public Renderer<numStrips, ledsPerStrip>
{
    public:
        WhiteWallRenderer() : Renderer<numStrips, ledsPerStrip>()
        {
            for (int s = 0; s < numStrips; s++)
            {
                Adafruit_NeoPixel *strip = this->getStrip(s);
                for (int ledIdx = 0; ledIdx < ledsPerStrip; ledIdx++)
                {
                    strip->setPixelColor(ledIdx, strip->ColorHSV(0xFF, 0, 0xFF));
                }
            }
        }

        ~WhiteWallRenderer()
        {
        }

        void render()
        {
        }


    private:
};

#endif // WHITEWALL_RENDERER_H_
