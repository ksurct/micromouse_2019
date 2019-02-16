/* localization.h */


#ifndef _CONTROL_H_
#define _CONTROL_H_


/* initialize control
 * starts the PID loop with a speed of 0 on each motor */
void initializeControl(void);

/* set speed PID
 * Sets the speed that the control algorithm uses */
void setSpeedPID(double left_speed, double right_speed);


#endif //_CONTROL_H_
