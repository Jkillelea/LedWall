#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "logging.h"
#include <cstdint>
#include <memory>
#include <Adafruit_NeoPixel.h>

extern Adafruit_NeoPixel g_LED_STRIPS[];

template<int numStrips, int ledsPerStrip>
class Renderer
{
    public:
        Renderer() {}
       ~Renderer() {}

        void render()
        {
            /* stub */
        }

        Adafruit_NeoPixel *getStrip(int n) {
            if (n < numStrips)
                return &g_LED_STRIPS[n];

            LOG(Log_Error, "Requested strip outside of bounds! Number:");
            LOG(Log_Error, n);
            LOG(Log_Error, " ");
            return NULL;
        }

    protected:
};

#endif // _RENDERER_H_
