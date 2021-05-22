#ifndef DROP_RENDERER_H_
#define DROP_RENDERER_H_
#include "Adafruit_NeoPixel.h"
#include "drop.h"
#include <renderer.hpp>
#include <Arduino.h>
#include <logging.h>

template<int numStrips, int ledsPerStrip>
class DropRenderer : public Renderer<numStrips, ledsPerStrip>
{
    public:
        DropRenderer() : Renderer<numStrips, ledsPerStrip>()
        {
            LOG(Log_Trace, "DropRenderer()");
        }

        ~DropRenderer() {}

        void render()
        {
            for (Drop &d : this->drops)
            {
                d.run(); // do physics
                int strip_num = (int) d.xPosition();
                int ypixel    = dropYIdktoLED((int) d.yPosition());
                int yspeed    = abs((int) d.yVelocity());

                if (ypixel >= 0 && strip_num < numStrips)
                {
                    Adafruit_NeoPixel *strip = this->getStrip(strip_num);

                    for (int tailpx = 0; tailpx <= ypixel; tailpx++)
                    {
                        int distance = ypixel - tailpx;
                        uint8_t brightness;

                        if (distance == 0)
                        {
                            brightness = 255;
                        }
                        else if (distance < ledsPerStrip)
                        {
                            brightness = (uint8_t) ((float) 10 * yspeed / sqrt(distance));
                        }
                        else
                        {
                            brightness = 0;
                        }
                            strip->setPixelColor(tailpx,
                                    strip->gamma32(
                                        strip->ColorHSV(this->hue, 255, brightness)));
                    }

                    // drop goes off bottom
                    if (ypixel > 2 * ledsPerStrip)
                    {
                        LOG(Log_Debug, String("Resetting drop on strip ") + strip_num);

                        this->hue += UINT16_MAX / 100;
                        d = Drop((double) random(0, numStrips),
                                 (double) random(0, 10 * ledsPerStrip),
                                -9.81);
                    }
                }

            }
        }

    private:
        Drop drops[25];

        int dropYIdktoLED(int ypos)
        {
            // If the drop is still off the top of the screen return -1
            if (ypos > 0)
            {
                return -1;
            }
            else
            {
                return abs(ypos);
            }
        }

};

#endif // DROP_RENDERER_H_
