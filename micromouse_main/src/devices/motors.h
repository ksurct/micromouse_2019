/* motors.h */

#include <Arduino.h>

#ifndef _MOTORS_H_
#define _MOTORS_H_


/* Sets up all of the motors */
void motorSetup();

/* Sets the motor PWM for the specified motor
 *   id: id of the motor, use either LEFT or RIGHT
 *   value: a number between 0 and 1 that represents
 *          the percentage of the time the signal
 *          should be on                          */
void setMotorPWM(uint8_t id, double value);

#endif //_MOTORS_H_
