/* control.h */


#ifndef _CONTROL_H_
#define _CONTROL_H_


/* initialize control
 * starts the PID loop with a speed of 0 on each motor */
void initializeControl(void);

/* distance travelled
 * The control subsystem keeps track of how far each wheel
 * has travelled, this function is provided so that the
 * main loop can get the distance travelled */
void distanceTravelled(double* left_distance, double* right_distance);

/* set speed PID
 * Sets the speed that the control algorithm uses */
void setSpeedPID(double left_speed, double right_speed);

/* speed controller
 * This function is a ISR to run the PID loop */
void speedController(void);


#endif //_CONTROL_H_
