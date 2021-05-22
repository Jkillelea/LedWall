#pragma once

#include "renderer.hpp"
#include <cstdint>
#include <cstring>

template<int numStrips, int ledsPerStrip>
class PixelRenderer : Renderer<numStrips, ledsPerStrip>
{
    public:
        PixelRenderer() : Renderer<numStrips, ledsPerStrip>()
        {
            memset(leds, 0, sizeof(leds[0][0]) * numStrips * ledsPerStrip);
        }

        ~PixelRenderer()
        {
        }

        void render()
        {
            // nothing
        }

        void setPixel(uint32_t x, uint32_t y, uint32_t val)
        {
            this->getStrip(x)->setPixelColor(val);
        }

    private:
        uint32_t leds[numStrips][ledsPerStrip];
};
