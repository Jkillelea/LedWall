#include "renderer.hpp"
#include "globals.hpp"

    template<int numStrips, int ledsPerStrip>
Renderer<numStrips, ledsPerStrip>::Renderer()
    : hue(0), sat(0), val(0)
{
}

    template<int numStrips, int ledsPerStrip>
Renderer<numStrips, ledsPerStrip>::~Renderer()
{
}

// Needs to be marked virtual for overloading
    template<int numStrips, int ledsPerStrip>
void Renderer<numStrips, ledsPerStrip>::render()
{
}

    template<int numStrips, int ledsPerStrip>
Adafruit_NeoPixel* Renderer<numStrips, ledsPerStrip>::getStrip(int n)
{
    if (n < numStrips)
        return &g_LED_STRIPS[n];

    LOG(Log_Error, "Requested strip outside of bounds! Number:");
    LOG(Log_Error, n);
    LOG(Log_Error, " ");
    return NULL;
}

    template<int numStrips, int ledsPerStrip>
void Renderer<numStrips, ledsPerStrip>::setHue(uint16_t h)
{
    this->hue = h;
}

    template<int numStrips, int ledsPerStrip>
void Renderer<numStrips, ledsPerStrip>::setSat(uint16_t s)
{
    this->sat = s;
}

    template<int numStrips, int ledsPerStrip>
void Renderer<numStrips, ledsPerStrip>::setVal(uint16_t v)
{
    this->val = v;
}
