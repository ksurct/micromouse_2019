/* control.h */


#ifndef _CONTROL_H_
#define _CONTROL_H_


#include "../types.h"


/* initialize control
 * starts the PID loop with a speed of 0 on each motor */
void initializeControl(void);

/* distance travelled
 * The control subsystem keeps track of how far each wheel
 * has travelled, this function is provided so that the
 * main loop can get the distance travelled */
void distanceTravelled(double* left_distance, double* right_distance);

/* calculate speed
 * Calculate the speed to set the motors to given the current_location and the next_location */
void calculateSpeed(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        double* left_speed, double* right_speed);

/* set speed PID
 * Sets the speed that the control algorithm uses */
void setSpeedPID(double left_speed, double right_speed);

/* speed controller
 * This function is a ISR to run the PID loop */
void speedController(void);


#endif //_CONTROL_H_
