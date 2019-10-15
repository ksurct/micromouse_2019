/* motors.h */


#ifndef _MOTORS_H_
#define _MOTORS_H_


/* Sets up all of the motors */
void motorSetup();

/* Sets the motor PWM for the specified motor
 *   id: id of the motor, use either LEFT or RIGHT
 *   value: a number between -1 and 1 that represents
 *   the amount to output */
void setMotorPWM(unsigned char id, double value);


#endif //_MOTORS_H_
