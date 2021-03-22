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
        DropRenderer()
            : hue(0)
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
                    // Adafruit_NeoPixel *strip = &g_LED_STRIPS[strip_num];
                    Adafruit_NeoPixel *strip = this->getStrip(strip_num);

                    // Set head to full brightness
                    strip->setPixelColor(ypixel, strip->gamma32(strip->ColorHSV(this->hue)));

                    for (int tailpx = 0; tailpx < ypixel; tailpx++)
                    {
                        int distance = ypixel - tailpx;
                        if (distance < ledsPerStrip)
                        {
                            strip->setPixelColor(tailpx,
                                    strip->gamma32(
                                        strip->ColorHSV(this->hue, 255, 10 * yspeed / sqrt(distance))));
                        }
                        else
                        {
                            strip->setPixelColor(tailpx, 0);
                        }
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
        uint16_t hue;
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
