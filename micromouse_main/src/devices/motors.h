/* motors.h */


#ifndef _MOTORS_H_
#define _MOTORS_H_

typedef struct {
    unsigned char id;
    unsigned char pinA;
    unsigned char pinB;
    double value;
} motor_t;

extern motor_t motors[];

/* Sets up all of the motors */
void motorSetup();

/* Sets the motor PWM for the specified motor
 *   id: id of the motor, use either LEFT or RIGHT
 *   value: a number between 0 and 1 that represents
 *          the percentage of the time the signal
 *          should be on                          */
void setMotorPWM(unsigned char id, double value);

#endif //_MOTORS_H_
