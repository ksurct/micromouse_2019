/* movement.cpp */


#include "movement.h"
#include "../types.h"
#include "../abs.h"


// Function Declarations

/*----------- Public Functions -----------*/

/* calculate speed
 * Calculate the speed to set the motors to given the current_location and the next_location
 * - left_speed and right_speed should be passed in with the current respective speeds*/
void calculateSpeed(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        double* left_speed, double* right_speed) {
    
    // Overall changes
    double x_cte = next_location->x_mu - current_location->x_mu;
    double y_cte = next_location->y_mu - current_location->y_mu;
    // double theta_change = next_location->theta_mu - current_location->theta_mu;

    // Deside wether this is a turn or a straight step
    signed char x_direction = (x_cte > 0) ? 1 : -1;
    signed char y_direction = (y_cte > 0) ? 1 : -1;
    //signed char theta_direction = (theta_change > 0) ? 1 : -1;

    // Assume going along x-axis
    if (abs(x_cte) < TOLERANCE_MM) {

    }
}

/*----------- Private Functions -----------*/

// None
