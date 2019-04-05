/* movement.cpp */


#include "movement.h"
#include "../types.h"
#include "../abs.h"


#define IS_BETWEEN_ERROR(x,y,e) (((x) < (y) + (e)) && ((x) > (y) - (e)))

// Globals
enum RobotState {
    OUT_XY_IN_THETA,            // Outside tolerance for one of x and y AND within tolerance for Theta
    OUT_XY_OUT_THETA,           // Outside tolerance for one of x and y AND outside tolerance for Theta
    IN_XY_IN_THETA,             // Within tolerance for one of x and y AND within tolerance for Theta
    IN_XY_OUT_THETA,            // Within tolerance for one of x and y AND outside tolerance for Theta
    PERFECT                     // Within tolerance for both x and y
};

enum Direction {
    North,  // North(0, -1) theta = 3*PI/2
    East,   // East(1, 0)   theta = 0
    South,  // South(0, 1)  theta = PI/2
    West,   // West(-1, 0)  theta = PI
};

double directionToRAD[4] = { 3*PI/2, 0, PI/2, PI };

double directionToXY[4][2] = { { 0, -1 }, { 1, 0 }, { 1, 0 }, { -1, 0 } };

gaussian_location_t prev_location;


// Function Declarations
RobotState currentState(gaussian_location_t* current_location, gaussian_location_t* next_location);


/*----------- Public Functions -----------*/

void initializeMovement(gaussian_location_t* current_location) {
    prev_location = *current_location;
}

/* calculate speed
 * Calculate the speed to set the motors to given the current_location and the next_location
 * - left_speed and right_speed should be passed in with the current respective speeds*/
void calculateSpeed(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        double* left_speed, double* right_speed) {
    
    RobotState state = currentState(current_location, next_location);

    switch (state) {
        case OUT_XY_IN_THETA:
            // Go Straight

            break;
        case OUT_XY_OUT_THETA:
            // Turn

            break;
        case IN_XY_IN_THETA:
            // Go Straight

            break;
        case IN_XY_OUT_THETA:
            // Turn

            break;
        case PERFECT:
            // Stop
            *left_speed = 0;
            *right_speed = 0;
            break;
    }

}

/*----------- Private Functions -----------*/
RobotState currentState(gaussian_location_t* current_location, gaussian_location_t* next_location) {
    
    // Overall changes
    double x_cte = next_location->x_mu - current_location->x_mu;
    double y_cte = next_location->y_mu - current_location->y_mu;
    
    if (abs(x_cte) > TOLERANCE_MM && abs(y_cte) > TOLERANCE_MM) {
        
        // Determine the direction to go
        Direction d;
        if (abs(x_cte) < abs(y_cte)) 
            d = x_cte > 0 ? East : West;
        else
            d = y_cte > 0 ? South : North;

        // Are we facing that direction?
        if (IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[d], TOLERANCE_RAD)) {
            return OUT_XY_IN_THETA;
        } else {
            return OUT_XY_OUT_THETA;
        }

    } else if (abs(x_cte) < TOLERANCE_MM && abs(y_cte) < TOLERANCE_MM) {
        
        return PERFECT;

    } else {

        // Determine the direction to go
        Direction d;
        if (abs(x_cte) < abs(y_cte)) 
            d = y_cte > 0 ? South : North;
        else
            d = x_cte > 0 ? East : West;

        // Are we facing that direction?
        if (IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[d], TOLERANCE_RAD)) {
            return IN_XY_IN_THETA;
        } else {
            return IN_XY_OUT_THETA;
        }
    }

    return PERFECT;
}

void straightController() {

}

void turnController() {

}
