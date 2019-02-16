/* motors.cpp */

#include "../settings.h"
#include "motors.h"


typedef struct {
    uint8_t pinA;
    uint8_t pinB;
    double value;
} motor_t;


/* Stores data about how to access each motor, left first, then right */
motor_t motors[] = {
    {
        .pinA = 1, // dummy value
        .pinB = 2, // dummy value
        .value = 0
    },
    {
        .pinA = 3, // dummy value
        .pinB = 4, // dummy value
        .value = 0
    },
};

void motorSetup()
{
    // dothething();
}

void setMotorPWM(uint8_t id, double value)
{
    // dothething();
}
