#ifndef _DROP_H_
#define _DROP_H_

#include <stdint.h>

class Drop {
    public:
        Drop(double x0 = 0, double y0 = 0, double gees = -9.81, uint32_t colorData = 0x00FFFFFF)
        {
            xpos = x0;
            ypos = y0;
            xvel = 0;
            yvel = 0;
            gravity = gees;
            lastTimeStampMs = millis();
            this->setColor(colorData);
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

        void setColor(uint32_t c)
        {
            this->colorData = c;
        }

        uint32_t color(void)
        {
            return this->colorData;
        }

    private:
    double xpos;
    double ypos;
    double xvel;
    double yvel;
    double gravity;
    uint64_t lastTimeStampMs;
    uint32_t colorData;
};

#endif // _DROP_H_
