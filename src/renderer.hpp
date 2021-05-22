#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "config.h"
#include "logging.h"
#include <cstdint>
#include <memory>
#include <Adafruit_NeoPixel.h>

// extern Adafruit_NeoPixel g_LED_STRIPS[NUM_STRIPS];

template<int numStrips, int ledsPerStrip>
class Renderer
{
    public:
        Renderer();

       ~Renderer();

        // Needs to be marked virtual for overloading
        virtual void render();

        Adafruit_NeoPixel *getStrip(int n);

        void setHue(uint16_t h);

        void setSat(uint16_t s);

        void setVal(uint16_t v);

    protected:
        uint16_t hue;
        uint16_t sat;
        uint16_t val;
};

#endif // _RENDERER_H_
