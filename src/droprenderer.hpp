#ifndef DROP_RENDERER_H_
#define DROP_RENDERER_H_
#include "Adafruit_NeoPixel.h"
#include "drop.h"
#include <cstring>
#include <renderer.hpp>
#include <Arduino.h>
#include <logging.h>
#include <vector>

template<int numStrips, int ledsPerStrip>
class DropRenderer : public Renderer<numStrips, ledsPerStrip>
{
    public:
        DropRenderer() : Renderer<numStrips, ledsPerStrip>()
        {
            LOG_TRACE("DropRenderer()");
        }

        ~DropRenderer() {}

        void render()
        {
            // For each strip, do calculations for drops on strip and store to pixel buffer
            for (int strip_num = 0; strip_num < numStrips; strip_num++)
            {
                LOG_TRACE(String("strip ") + strip_num);

                LOG_TRACE("memset");
                memset(this->ledBuffer, 0, sizeof(ledBuffer));

                Adafruit_NeoPixel *strip = &g_LED_STRIPS[strip_num];

                // handle each drop that could be on this strip
                for (Drop &d : this->drops)
                {
                    if (((int) d.xPosition()) == strip_num) // skip drops not on this strip
                    {
                        d.run();
                        int ypixel    = dropYIdktoLED((int) d.yPosition());
                        int yspeed    = abs((int) d.yVelocity());

                        if (ypixel >= 0)
                        {
                            // Draw teardrop tail
                            for (int tailpx = 0; tailpx <= ypixel; tailpx++)
                            {
                                int distance = ypixel - tailpx;
                                uint8_t brightness = 0;

                                if (distance == 0)
                                {
                                    brightness = 255;
                                }
                                else if (distance < ledsPerStrip)
                                {
                                    brightness = (uint8_t) ((float) 10 * yspeed / sqrt(distance));
                                }

                                this->ledBuffer[tailpx] +=
                                    strip->gamma32(strip->ColorHSV(this->hue,
                                                this->sat,
                                                brightness));
                            }
                            yield();
                        }

                        // drop goes off bottom
                        if (ypixel > 2 * ledsPerStrip)
                        {
                            LOG_DEBUG(String("Resetting drop on strip ") + strip_num);

                            this->hue += UINT16_MAX / 100;
                            d = Drop((double) random(0, numStrips),
                                    (double) random(0, 10 * ledsPerStrip),
                                    -9.81);
                        }
                    }
                }


                LOG_TRACE("DropRenderer::render - write to pixels");
                for (int led = 0; led < ledsPerStrip; led++)
                {
                    uint32_t ledColor = this->ledBuffer[led];
                    strip->setPixelColor(led, ledColor);
                }
            } // end for each strip

            LOG_TRACE("DropRenderer::render - rendered to strips");
        }

    private:
        Drop drops[20];
        uint32_t ledBuffer[ledsPerStrip];

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
