#ifndef _DROP_H_
#define _DROP_H_

#include <stdint.h>

class Drop {
    public:
        Drop(double x0 = 0, double y0 = 0, double gees = -9.81,
             uint8_t r = 128, uint8_t g = 128, uint8_t b = 128)
        {
            xpos = x0;
            ypos = y0;
            xvel = 0;
            yvel = 0;
            gravity = gees;
            lastTimeStampMs = millis();
            this->setColor(r, g, b);
        }

        void run()
        {
            uint64_t now = millis();
            double deltaSeconds = ((double) (now - lastTimeStampMs)) / 1000.0;

            xpos += xvel * deltaSeconds;
            ypos += yvel * deltaSeconds;

            xvel += 0;
            yvel += gravity * deltaSeconds;

            lastTimeStampMs = now;
        }

        double xPosition() {
            return xpos;
        }

        double yPosition() {
            return ypos;
        }

        double yVelocity() {
            return yvel;
        }

        void setColor(uint8_t r, uint8_t g, uint8_t b)
        {
            this->colors[0] = r;
            this->colors[1] = g;
            this->colors[2] = b;
        }

        uint8_t color(int idx)
        {
            int color_count = sizeof(this->colors) / sizeof(this->colors[0]);
            if (idx < color_count);
                return this->colors[idx];

            return 255;
        }

    private:
    double xpos;
    double ypos;
    double xvel;
    double yvel;
    double gravity;
    uint64_t lastTimeStampMs;
    uint8_t colors[3];
};

#endif // _DROP_H_
