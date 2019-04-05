/* movement.h */


#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_


#include "../types.h"


/* calculate speed
 * Calculate the speed to set the motors to given the current_location and the next_location */
void calculateSpeed(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        double* left_speed, double* right_speed);

#endif //_MOVEMENT_H_
