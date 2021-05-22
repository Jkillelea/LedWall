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
            // Zero buffer
            memset(this->ledBuffer, 0, sizeof(ledBuffer[0])*NUM_STRIPS*LEDS_PER_STRIP);

            // For each drop in memory, do calculations and store to pixel buffer
            for (Drop &d : this->drops)
            {
                d.run(); // do physics calculations
                int strip_num = (int) d.xPosition();
                int ypixel    = dropYIdktoLED((int) d.yPosition());
                int yspeed    = abs((int) d.yVelocity());

                if (ypixel >= 0 && strip_num < numStrips)
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

                        this->ledBuffer[strip_num][tailpx] =
                            g_LED_STRIPS[0].gamma32(
                                    g_LED_STRIPS[0].ColorHSV(this->hue,
                                                             this->sat,
                                                             brightness));
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
            } // end for each drop

            // Render the buffer to LED strips
            for (int strip_num = 0; strip_num < NUM_STRIPS; strip_num++)
            {
                Adafruit_NeoPixel *strip = this->getStrip(strip_num);

                if (strip != NULL)
                    for (int led = 0; led < LEDS_PER_STRIP; led++)
                        strip->setPixelColor(led, this->ledBuffer[strip_num][led]);
            }

        }

    private:
        Drop drops[25];
        // std::vector<Drop> drops;
        uint32 ledBuffer[NUM_STRIPS][LEDS_PER_STRIP];

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
