/* motors.h */

#ifndef _MOTORS_H_
#define _MOTORS_H_

typedef struct {
    uint8_t id;
    uint8_t pinA;
    uint8_t pinB;
    double value;
} motor_t;

extern motor_t motors[];

/* Sets up all of the motors
 *   returns: true if successfull */
bool motorSetup();

/* Sets the motor PWM for the specified motor
 *   id: id of the motor, use either LEFT or RIGHT
 *   value: a number between 0 and 1 that represents
 *          the percentage of the time the signal
 *          should be on
 *   returns: true is successfull */
bool setMotorPWM(uint8_t id, double value);

#endif //_MOTORS_H_
