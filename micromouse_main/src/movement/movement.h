/* movement.h */


#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_


#include "../types.h"


// Globals
enum RobotState {
    PERFECT,                    // Within tolerance for both x and y
    OUT_XY_IN_THETA,            // Outside tolerance for one of x and y AND within tolerance for Theta
    OUT_XY_OUT_THETA,           // Outside tolerance for one of x and y AND outside tolerance for Theta
    IN_XY_IN_THETA,             // Within tolerance for one of x and y AND within tolerance for Theta
    IN_XY_OUT_THETA             // Within tolerance for one of x and y AND outside tolerance for Theta
};

// Check is x is between y+e and y-e
#define IS_BETWEEN_ERROR(x,y,e) (((x) < (y) + (e)) && ((x) > (y) - (e)))


/* calculate speed
 * Calculate the speed to set the motors to given the current_location and the next_location
 * - left_speed and right_speed should be passed in with the current respective speeds*/
void calculateSpeed(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        double* left_speed, double* right_speed);

#endif //_MOVEMENT_H_
