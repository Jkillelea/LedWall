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

                    LOG(Log_Trace, String("strip: ")  + strip_num);
                    LOG(Log_Trace, String("ypos: ")   + d.yPosition());
                    LOG(Log_Trace, String("ypixel: ") + ypixel);

                    strip->setPixelColor(ypixel, strip->gamma32(d.color()));

                    for (int tail = 0; tail < ypixel; tail++)
                    {
                        int distance = ypixel - tail;
                        LOG(Log_Trace, String("distance: ") + distance);

                        if (distance < ledsPerStrip)
                        {
                            strip->setPixelColor(tail,
                                    strip->gamma32(
                                        strip->ColorHSV(this->hue, 255, 10 * yspeed / sqrt(distance))));
                        }
                        else
                        {
                            strip->setPixelColor(tail, 0);
                        }
                    }

                    // drop goes off bottom
                    if (ypixel > 2 * ledsPerStrip)
                    {
                        LOG(Log_Trace, String("Resetting drop on strip ") + strip_num);

                        strip->clear();

                        this->hue += UINT16_MAX / 100;
                        uint32_t color = strip->ColorHSV(this->hue);
                        d = Drop((double) random(0, numStrips),
                                (double) random(0, 10 * ledsPerStrip),
                                -9.81,
                                color);
                    }
                }

            }
        }

    private:
        uint16_t hue;
        Drop drops[10];

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
