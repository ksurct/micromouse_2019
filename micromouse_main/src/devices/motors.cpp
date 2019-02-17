/* motors.cpp */


#include "../settings.h"
#include "motors.h"


motor_t motors[] = {
    {
        .id = LEFT,
        .pinA = 1, // dummy value
        .pinB = 2, // dummy value
        .value = 0
    },
    {
        .id = RIGHT,
        .pinA = 3, // dummy value
        .pinB = 4, // dummy value
        .value = 0
    },
};

void motorSetup()
{
    // dothething();
}

void setMotorPWM(unsigned char id, double value)
{
    // dothething();
}
