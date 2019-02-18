/* motors.cpp */


#include <Arduino.h>

#include "motors.h"
#include "../settings.h"


typedef struct {
    unsigned char pinA;
    unsigned char pinB;
    double value;
} motor_t;


/* Stores data about how to access each motor, left first, then right */
motor_t motors[] = {
    {
        .pinA = LEFT_MOTOR_PIN_A,
        .pinB = LEFT_MOTOR_PIN_B,
        .value = 0
    },
    {
        .pinA = RIGHT_MOTOR_PIN_A,
        .pinB = RIGHT_MOTOR_PIN_B,
        .value = 0
    },
};

/* Sets up all of the motors */
void motorSetup() {
    analogWriteResolution(RESOLUTION_BITS);

    for (int i = 0; i < 2; i++) {
        pinMode(motors[i].pinA, OUTPUT);
        pinMode(motors[i].pinB, OUTPUT);
        setMotorPWM(i, 0.0);
    }
}

/* Sets the motor PWM for the specified motor
 *   id: id of the motor, use either LEFT or RIGHT
 *   value: a number between -1 and 1 that represents
 *   the amount to output */
void setMotorPWM(unsigned char id, double value) {
    int output = (int) (value * MAX_PWM_OUTPUT);
    
    if (output < 0) {
        analogWrite(motors[id].pinA, 0);
        analogWrite(motors[id].pinB, -output);
    } else {
        analogWrite(motors[id].pinA, output);
        analogWrite(motors[id].pinB, 0);
    }
}
