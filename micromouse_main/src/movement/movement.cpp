/* movement.cpp 
 * 
 * This file is used to calculate the speed to set the motors at to get closer to the next location
 * 
 * Model:
 * C: current_location - the current state of the robot
 * N: next_location - goal location, where we want to be
 * I: intermediate_location - Not always used, but the location we should travel to first before
 *      attempting to go to the next_location
 * v and ->: path of robot, goes down then over because the error in x is less than the error in y
 * 
 * Axis Definition:
 * +--> +X                                      N.x
 * |                                             |
 * V        C                                    |
 * +Y       v                                    |
 *          v                                    |
 *          v                                    |
 *   - - - -v- - - - - - - - - - - - - - - - - - | - - - -
 *          v                                    |       ^
 *          v                                    |       OUTER_TOLERANCE_MM
 *          v                                    |       v
 *  --------I->->->->->->->->->->->->->->->->->->N-------- N.y
 *                                               |       ^
 *                                               |       OUTER_TOLERANCE_MM
 *                                               |       v
 *   - - - - - - - - - - - - - - - - - - - - - - | - - - -
 *
 * Main ideas:
 * - Get to a point where you are within the goal tolerance for one axis before try to drive there
 * - Either call straightController() or turnController() each time based on what state we are determined to be in
 * - Once within tolerance for both x and y stop motors completely
 * 
 * */


#include "movement.h"
#include "../types.h"
#include "../settings.h"
#include "../abs.h"

// Temp
#include <stdio.h>


// Globals
gaussian_location_t prev_location;
RobotState prev_state;
Direction prev_direction;


// Function Declarations
bool canSwitchState(gaussian_location_t *current_location, gaussian_location_t *next_location);
void getCurrentState(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        Direction *dir, RobotState *state);
Direction chooseDirectionOutOfTolerance(double x_cte, double y_cte);
Direction chooseDirectionWithinTolerance(double x_cte, double y_cte);
void straightController(gaussian_location_t* current_location, gaussian_location_t* next_location,
                            double* left_speed, double* right_speed, Direction dir, bool same_state);
double calculateCTE(gaussian_location_t* cur, gaussian_location_t* next, Direction dir);
double calculateThetaCTE(gaussian_location_t* cur, Direction dir, double cte);
double calculateDistanceAway(gaussian_location_t* cur, gaussian_location_t* next, Direction dir);
double straightSpeedProfile(double distance_away);

void turnController(gaussian_location_t* current_location, double* left_speed,
                            double* right_speed, Direction dir, bool same_state);
double calculateThetaError(gaussian_location_t* cur, Direction dir);
double turnSpeedProfile(double thetaError);


/*----------- Public Functions -----------*/

void initializeMovement(gaussian_location_t* current_location) {
    prev_location = *current_location;
    prev_state = PERFECT;
}

/* calculate speed
 * Calculate the speed to set the motors to given the current_location and the next_location
 * - left_speed and right_speed should be passed in with the current respective speeds*/
void calculateSpeed(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        double* left_speed, double* right_speed) {
    
    RobotState current_state;
    Direction direction;

    // Get the new current state if we are allowed to check
    if (canSwitchState(current_location, next_location)) {
        getCurrentState(current_location, next_location, &direction, &current_state);
    } else {
        current_state = prev_state;
        direction = prev_direction;
    }

    bool same_state = (current_state == prev_state);

    /* Bad Code - This is a hack and this is really bad code, but it might actually help with death spikes */
    
    // After get done turning, set theta to be perfectly aligned
    if (current_state == OUT_XY_IN_THETA && prev_state == OUT_XY_OUT_THETA ||
            current_state == IN_XY_IN_THETA && prev_state == IN_XY_OUT_THETA) {
        
        current_location->theta_mu = directionToRAD[direction];
    }

    /* end of Bad Code */

    // printf("State : %d, Same: %d, Direction: %d\n", current_state, same_state, direction);

    switch (current_state) {
        default:
        case PERFECT:
            // Stop
            *left_speed = 0;
            *right_speed = 0;
            break;

        case OUT_XY_IN_THETA:
            // Go Straight to get to within the tolerance

            // We need to find the location that is on the same axis as the next_location
            // in the direction we are being told to go
            gaussian_location_t intermediate_location;

            // To make sure that the axis for error doesn't drift
            static double prev_axis_x; static double prev_axis_y;
            double axis_x; double axis_y;
            if (same_state) {
                axis_x = prev_axis_x;
                axis_y = prev_axis_y;
            } else {
                axis_x = prev_location.x_mu;
                axis_y = prev_location.y_mu;
            }
            // printf("same state: %d, axis_x: %f, axis_y: %f direction: %d\n", same_state, axis_x, axis_y, direction);

            // Direction does not depend on x, set to stay the same as previous location to keep same axis for cte
            if (directionToXY[direction][0] == 0)
                intermediate_location.x_mu = axis_x;
            else // Direction does depend on x, project to axis by setting to next location
                intermediate_location.x_mu = next_location->x_mu;

            // Direction does not depend on y, set to stay the same as previous location to keep same axis for cte
            if (directionToXY[direction][1] == 0)
                intermediate_location.y_mu = axis_y;
            else // Direction does depend on y, project to axis by setting to next location
                intermediate_location.y_mu = next_location->y_mu;

            // printf("i location: (%f, %f, %f)\n", intermediate_location.x_mu, intermediate_location.y_mu, intermediate_location.theta_mu);

            straightController(current_location, &intermediate_location, left_speed, right_speed, direction, same_state);

            prev_axis_x = axis_x;
            prev_axis_y = axis_y;
            break;

        case OUT_XY_OUT_THETA:
            
            // Turn to get to within the tolerance of direction
            turnController(current_location, left_speed, right_speed, direction, same_state);
            break;

        case IN_XY_IN_THETA:
            
            // Go Straight
            straightController(current_location, next_location, left_speed, right_speed, direction, same_state);
            break;
            
        case IN_XY_OUT_THETA:
            
            // Turn to get to within the tolerance of direction
            turnController(current_location, left_speed, right_speed, direction, same_state);
            break;
    }

    prev_location = *current_location;
    prev_state = current_state;
    prev_direction = direction;
}

bool canSwitchState(gaussian_location_t *current_location, gaussian_location_t *next_location) {

    // Overall changes
    double x_cte = next_location->x_mu - current_location->x_mu;
    double y_cte = next_location->y_mu - current_location->y_mu;

    // Check for transition conditions
    switch (prev_state) {
        default:
        case PERFECT:
            return true;
            break;

        case OUT_XY_IN_THETA:
            // To change states:
            // either x or y needs to be within inner tolerance
            if (abs(x_cte) < INNER_TOLERANCE_MM || abs(y_cte) < INNER_TOLERANCE_MM)
                return true;
            // or theta needs to be outside outer tolerance
            if (prev_direction != East){
                if (!IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[prev_direction], OUTER_TOLERANCE_RAD))
                    return true;
            } else { // Special case because circles
                if ( !(((current_location->theta_mu <= TWO_PI) && (current_location->theta_mu > TWO_PI - OUTER_TOLERANCE_RAD))
                        || ((current_location->theta_mu < OUTER_TOLERANCE_RAD) && (current_location->theta_mu >= 0.0))))
                    return true;
            }
            break;

        case OUT_XY_OUT_THETA:
            // To change states:
            // either x or y needs to be within inner tolerance 
            if (abs(x_cte) < INNER_TOLERANCE_MM || abs(y_cte) < INNER_TOLERANCE_MM)
                return true;
            // or theta needs to be inside inner tolerance
            if (prev_direction != East){
                if (IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[prev_direction], INNER_TOLERANCE_RAD))
                    return true;
            } else { // Special case because circles
                if (((current_location->theta_mu <= TWO_PI) && (current_location->theta_mu > TWO_PI - INNER_TOLERANCE_RAD))
                        || ((current_location->theta_mu < INNER_TOLERANCE_RAD) && (current_location->theta_mu >= 0.0)))
                    return true;
            }
            break;

        case IN_XY_IN_THETA:
            // To change states:
            // both x and y must be outside of outer tolerance
            if (abs(x_cte) > OUTER_TOLERANCE_MM && abs(y_cte) > OUTER_TOLERANCE_MM)
                return true;
            // or both x and y must be within inner tolerance
            if (abs(x_cte) < INNER_TOLERANCE_MM || abs(y_cte) < INNER_TOLERANCE_MM)
                return true;
            // or theta needs to be outside outer tolerance
            if (prev_direction != East){
                if (!IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[prev_direction], OUTER_TOLERANCE_RAD))
                    return true;
            } else { // Special case because circles
                if ( !(((current_location->theta_mu <= TWO_PI) && (current_location->theta_mu > TWO_PI - OUTER_TOLERANCE_RAD))
                        || ((current_location->theta_mu < OUTER_TOLERANCE_RAD) && (current_location->theta_mu >= 0.0))))
                    return true;
            }
            break;

        case IN_XY_OUT_THETA:
            // To change states:
            // both x and y must be outside of outer tolerance
            if (abs(x_cte) > OUTER_TOLERANCE_MM && abs(y_cte) > OUTER_TOLERANCE_MM)
                return true;
            // or both x and y must be within inner tolerance
            if (abs(x_cte) < INNER_TOLERANCE_MM || abs(y_cte) < INNER_TOLERANCE_MM)
                return true;
            // or theta needs to be within inner tolerance
            if (prev_direction != East){
                if (IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[prev_direction], INNER_TOLERANCE_RAD))
                    return true;
            } else { // Special case because circles
                if (((current_location->theta_mu <= TWO_PI) && (current_location->theta_mu > TWO_PI - INNER_TOLERANCE_RAD))
                        || ((current_location->theta_mu < INNER_TOLERANCE_RAD) && (current_location->theta_mu >= 0.0)))
                    return true;
            }
            break;
    }
    return false;
}


void getCurrentState(gaussian_location_t* current_location, gaussian_location_t* next_location,
                        Direction *dir, RobotState *state) {
    
    // Overall changes
    double x_cte = next_location->x_mu - current_location->x_mu;
    double y_cte = next_location->y_mu - current_location->y_mu;
    
    if (abs(x_cte) > OUTER_TOLERANCE_MM && abs(y_cte) > OUTER_TOLERANCE_MM) {
        
        // Determine the direction to go
        *dir = chooseDirectionOutOfTolerance(x_cte, y_cte);

        // Are we facing that direction?
        if (IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[*dir], OUTER_TOLERANCE_RAD)) {
            *state = OUT_XY_IN_THETA;
        } else {
            *state = OUT_XY_OUT_THETA;
        }

    } else if (abs(x_cte) < OUTER_TOLERANCE_MM && abs(y_cte) < OUTER_TOLERANCE_MM) {
        
        *state = PERFECT;

    } else {

        // Determine the direction to go
        *dir = chooseDirectionWithinTolerance(x_cte, y_cte);

        // Are we facing that direction?
        if (*dir != East){
            if (IS_BETWEEN_ERROR(current_location->theta_mu, directionToRAD[*dir], OUTER_TOLERANCE_RAD)) {
                *state = IN_XY_IN_THETA;
            } else {
                *state = IN_XY_OUT_THETA;
            }
        } else { // Special case because circles
            if (((current_location->theta_mu <= TWO_PI) && (current_location->theta_mu > TWO_PI - OUTER_TOLERANCE_RAD))
                    || ((current_location->theta_mu < OUTER_TOLERANCE_RAD) && (current_location->theta_mu >= 0.0))) {
                *state = IN_XY_IN_THETA;
            } else {
                *state = IN_XY_OUT_THETA;
            }
        }
    }
}

Direction chooseDirectionOutOfTolerance(double x_cte, double y_cte) {
    if (abs(x_cte) < abs(y_cte)) 
        return x_cte > 0 ? East : West;
    else
        return y_cte > 0 ? South : North;
}

Direction chooseDirectionWithinTolerance(double x_cte, double y_cte) {
    if (abs(x_cte) < abs(y_cte)) 
        return y_cte > 0 ? South : North;
    else
        return x_cte > 0 ? East : West;
}


/* Controllers - tune in settings.h */

void straightController(gaussian_location_t* current_location, gaussian_location_t* next_location,
                            double* left_speed, double* right_speed, Direction dir, bool same_state) {
    
    static double int_cte = 0;    // Integral error (Resets when same_state is false)
    static double prev_cte = 0;   // Previous error (Resets when same_state is false)

    // Calculate the Cross Track Error (cte)
    double cte = calculateCTE(current_location, next_location, dir);
    // printf("CTE: %f\n", cte);
    double d_cte; // derivative of cte

    if (same_state) {
        int_cte += cte;
        d_cte = prev_cte - cte;
    } else {
        int_cte = cte;
        d_cte = 0;
    }

    // How far off is theta
    double theta_cte = calculateThetaCTE(current_location, dir, cte);

    // The amount we should try to rotate to the left to get back to the correct location (PID)
    double rotate_left = (-1 * STRAIGHT_TAU_P * cte) +
                            (-1 * STRAIGHT_TAU_I * int_cte) +
                            (-1 * STRAIGHT_TAU_D * d_cte) +
                            (STRAIGHT_TAU_THETA * theta_cte);


    // Determine the base straight forward speed
    double distance_away = calculateDistanceAway(current_location, next_location, dir);
    
    double base_speed = straightSpeedProfile(distance_away);

    // Set the speed the motors should be at
    *left_speed = base_speed + rotate_left;
    *right_speed = base_speed - rotate_left;

}

double calculateCTE(gaussian_location_t* cur, gaussian_location_t* next, Direction dir) {

    switch (dir) {

        case North:  // North(0, -1) theta = 3*PI/2
            return (cur->x_mu - next->x_mu);
        
        case East:   // East(1, 0)   theta = 0
            return (cur->y_mu - next->y_mu);

        case South:  // South(0, 1)  theta = PI/2
            return -1 * (cur->x_mu - next->x_mu);

        case West:   // West(-1, 0)  theta = PI
            return -1 * (cur->y_mu - next->y_mu);
        
        default:
            return 0;
    }
}

double calculateThetaCTE(gaussian_location_t* cur, Direction dir, double cte) {

    double goal = directionToRAD[dir];
    double theta_cte;

    if (dir == East) {
        // Special case
        if (0 <= cur->theta_mu && cur->theta_mu <= PI) {
            theta_cte = 0.0 - cur->theta_mu;
        } else {
            theta_cte = TWO_PI - cur->theta_mu;
        }

    } else {
        // Normal case
        theta_cte = directionToRAD[dir] - cur->theta_mu;
    }
    return theta_cte;
}

double calculateDistanceAway(gaussian_location_t* cur, gaussian_location_t* next, Direction dir) {

    switch (dir) {

        case North:  // North(0, -1) theta = 3*PI/2
            return -1 * (next->y_mu - cur->y_mu);
        
        case East:   // East(1, 0)   theta = 0
            return (next->x_mu - cur->x_mu);

        case South:  // South(0, 1)  theta = PI/2
            return (next->y_mu - cur->y_mu);

        case West:   // West(-1, 0)  theta = PI
            return -1 * (next->x_mu - cur->x_mu);
        
        default:
            return 0;
    }
}

/*
 *                     SLOPE
 *                       v   Goal
 * STABLE_SPEED----------\    |
 *                         \  | 
 *                           \|-INTERCEPT 
 *                            |
 * ===========================|===========> distance_away
 */
double straightSpeedProfile(double distance_away) {

    // Implement the function in the block comment above
    if (distance_away > 0) {
        return min((STRAIGHT_PROFILE_SLOPE * distance_away) + STRAIGHT_PROFILE_INTERCEPT, STRAIGHT_PROFILE_STABLE_SPEED);
    } else {
        return max((STRAIGHT_PROFILE_SLOPE * distance_away) + STRAIGHT_PROFILE_INTERCEPT, -1 * STRAIGHT_PROFILE_STABLE_SPEED);
    }
}

// TODO: Everything below this

/* Turn toward the correct direction */
void turnController(gaussian_location_t* current_location, double* left_speed,
                            double* right_speed, Direction dir, bool same_state) {

    // Error between theta and the desired direction
    double thetaError = calculateThetaError(current_location, dir);

    // The amount we should try to rotate to the left to get back to the correct location (PID)
    double rotate_left = turnSpeedProfile(thetaError);

    *left_speed = rotate_left;
    *right_speed = -1 * rotate_left;
}


double calculateThetaError(gaussian_location_t* cur, Direction dir) {
    double thetaError = directionToRAD[dir] - cur->theta_mu;

    // limit final theta between 0 and 2 pi
    while (thetaError < 0) { thetaError += TWO_PI; }
    while (thetaError >= TWO_PI) { thetaError -= TWO_PI; }

    return thetaError;
}


/*
 * This function defines the speed profile to be used while turning
 * 
 *     rotate_left
 *          |      /--------------------------STABLE_SPEED                            |
 *          |    /                                                                    |
 *          |  / <- SLOPE                                                             |
 *INTERCEPT-|/                                                                        |
 *          |                                                                         |
 *          |0                                pi                                   2pi|
 *     -----|---------------------------------|---------------------------------------|-> thetaError
 *          |                                                                         |
 *          |                                                                         |
 *          |                                                                        /|- -1*INTERCEPT
 *          |                                                                      /  |
 *          |                                                           SLOPE -> /    |
 *          |                   -1*STABLE_SPEED--------------------------------/      |
 */
double turnSpeedProfile(double thetaError) {

    // Implement the function in the block comment above
    if (0 < thetaError && thetaError <= PI) {
        // printf("thetaError: %f\n", thetaError);
        // printf("Turn: %f\n", (TURN_PROFILE_SLOPE * thetaError) + TURN_PROFILE_INTERCEPT);
        return min((TURN_PROFILE_SLOPE * thetaError) + TURN_PROFILE_INTERCEPT, TURN_PROFILE_STABLE_SPEED);
    } else if (PI < thetaError && thetaError < TWO_PI) {
        // printf("thetaError: %f\n", thetaError);
        // printf("Turn: %f\n", (TURN_PROFILE_SLOPE * (thetaError - TWO_PI)) - TURN_PROFILE_INTERCEPT);
        return max((TURN_PROFILE_SLOPE * (thetaError - TWO_PI)) - TURN_PROFILE_INTERCEPT, -1 * TURN_PROFILE_STABLE_SPEED);
    } else {
        // printf("Turn: 0");
        return 0;
    }
}
