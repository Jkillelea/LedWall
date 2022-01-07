#ifndef SOCKET_RENDERER_H_
#define SOCKET_RENDERER_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <renderer.hpp>
#include <logging.h>

template<int numStrips, int ledsPerStrip>
class SocketRenderer : public Renderer<numStrips, ledsPerStrip>
{
    public:
        SocketRenderer() : Renderer<numStrips, ledsPerStrip>()
        {
        }

        ~SocketRenderer() {}

        void render()
        {
        }
    private:
};


#endif // SOCKET_RENDERER_H_
