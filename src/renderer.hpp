#pragma once

#include "config.h"
#include "logging.h"
#include <cstdint>
#include <memory>
#include <Adafruit_NeoPixel.h>

// Cannot include globals.h without creating recursive includes
extern Adafruit_NeoPixel g_LED_STRIPS[NUM_STRIPS];

/* Template class declarations must be all in header files for some reason */
template<int numStrips, int ledsPerStrip>
class Renderer
{
    public:
        Renderer() : hue(0), sat(0), val(0) { }

       ~Renderer() { }

        // Needs to be marked virtual for overloading
        virtual void render() { }

        Adafruit_NeoPixel *getStrip(int n)
        {
            if (n < numStrips)
                return &g_LED_STRIPS[n];

            LOG(Log_Error, "Requested strip outside of bounds! Number:");
            LOG(Log_Error, n);
            LOG(Log_Error, " ");
            return NULL;
        }

        void setHue(uint16_t h)
        {
            this->hue = h;
        }

        void setSat(uint8_t s)
        {
            this->sat = s;
        }

        void setVal(uint8_t v)
        {
            this->val = v;
        }

    protected:
        uint16_t hue;
        uint8_t  sat;
        uint8_t  val;
};

