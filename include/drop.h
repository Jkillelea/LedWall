#ifndef _DROP_H_
#define _DROP_H_

class Drop {
    public:
        Drop(double x0 = 0, double y0 = 0, double gees = -9.81)
        {
            xpos = x0;
            ypos = y0;
            xvel = 0;
            yvel = 0;
            gravity = gees;
            lastTimeStampMs = millis();
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

    private:
    double xpos;
    double ypos;
    double xvel;
    double yvel;
    double gravity;
    uint64_t lastTimeStampMs;
};

#endif // _DROP_H_
