/* localization.cpp */


#include <math.h>

#include "localization.h"
#include "../settings.h"
#include "../types.h"
#include "../abs.h"

// Temp
#include <stdio.h>


#define ENCODER_VARIANCE(d) (((ENCODER_VARIANCE_PER_MM) * abs(d)) + (ENCODER_VARIANCE_BASE))


// Globals
probabilistic_maze_t robot_maze_state;
gaussian_location_t robot_location;


/*----------- Public Functions -----------*/

/* initialize localization */
void initializeLocalization() {

    // Initialize robot_maze_state
    initializeMaze(&robot_maze_state);

    // Initialize robot_location
    robot_location.x_mu = INIT_X_MU;
    robot_location.x_sigma = INIT_X_SIGMA;
    robot_location.xy_sigma = INIT_XY_SIGMA;
    robot_location.y_mu = INIT_Y_MU;
    robot_location.y_sigma = INIT_Y_SIGMA;
    robot_location.theta_mu = INIT_THETA_MU;
    robot_location.theta_sigma = INIT_THETA_SIGMA;
}

/* Localize Motion Step
 * - Updates the global robot_location based on the motion recorded from the left and right wheels */
gaussian_location_t* localizeMotionStep(double left_distance, double right_distance) {

    // Calculate the motion mean and covariance matrix
    gaussian_location_t motion;
    calculateMotion(&motion, left_distance, right_distance);
    
    // Update the current location and covariance matrix by adding in the distance travelled
    addMotion(&motion);
}

/* Maze Mapping
 * - Updates the global robot_maze_state based on the believed current position and the sensor data recorded */
probabilistic_maze_t* mazeMapping(sensor_reading_t* sensor_data) {
    // dothething();
}

/* Localize Measure Step
 * - Updates the global robot_location based on current position, current map state and the sensor data recorded */
gaussian_location_t* localizeMeasureStep(sensor_reading_t* sensor_data) {
    // dothething();
}


/*----------- Private Functions -----------*/

void calculateMotion(gaussian_location_t* motion, double left_distance, double right_distance) {

    /* Run the motion through the system model */

    // printf("CalculateMotion: \t%f \t%f \t%d \n", left_distance, right_distance, left_distance==right_distance);
    
    // If going straight
    if (left_distance != right_distance) {
        
        // R = (l/2)*((dr + dl)./(dr - dl));
        double turn_radius = (WHEEL_BASE_LENGTH/2) * ((right_distance + left_distance) / (right_distance - left_distance));

        // change_in_theta = (dr - dl)/l;
        double delta_theta = (right_distance - left_distance) / WHEEL_BASE_LENGTH;

        // x = R .* sin(change_in_theta);
        motion->x_mu = turn_radius * sin(delta_theta);

        // y = -1 .* R .* cos(change_in_theta) + R;
        motion->y_mu = -1 * turn_radius * cos(delta_theta) + turn_radius;

        // y = -1 .* y;
        motion->y_mu = -1 * motion->y_mu; // our axis are setup inverted in the y direction

        // theta = 2*pi - change_in_theta;
        motion->theta_mu = TWO_PI - delta_theta; // our axis are setup inverted in the y direction

    } else { // Special case when going perfectly straight

        motion->x_mu = left_distance;
        motion->y_mu = 0.0;
        motion->theta_mu = 0.0;

    }
    
    /* Calculate the change in the covariance matrix */
    
    double distance_travelled = (left_distance + right_distance) / 2;

    motion->x_sigma = X_VARIANCE * ENCODER_VARIANCE(distance_travelled);
    motion->y_sigma = Y_VARIANCE * ENCODER_VARIANCE(distance_travelled);
    motion->xy_sigma = 0;
    motion->theta_sigma = THETA_VARIANCE * ENCODER_VARIANCE(distance_travelled);

    // If not straight
    if (left_distance != right_distance)
        // rotate covariance by motion->theta_mu
        rotateCovariance(motion->theta_mu, &motion->x_sigma, &motion->y_sigma, &motion->xy_sigma);

}

/* Rotate the covariance matrix described by x_sigma, y_sigma, and xy_sigma
 * Assumes xy_sigma starts as 0 */
void rotateCovariance(double rotate_by, double* x_sigma, double* y_sigma, double* xy_sigma) {

    /* Rotation matrix
        R:  [r1, r2]
            [r3, r4]    */
    double r1 = cos(rotate_by);
    double r2 = -1 * sin(rotate_by);
    double r3 = sin(rotate_by);
    double r4 = cos(rotate_by);

    // Rotate with: R * Sigma * R'
    double x_temp = *x_sigma;
    double y_temp = *y_sigma;

    *x_sigma = (r1 * r1 * (x_temp)) + (r2 * r2 * (y_temp));
    *y_sigma = (r3 * r3 * (x_temp)) + (r4 * r4 * (y_temp));
    *xy_sigma = (r1 * r3 * (x_temp)) + (r2 * r4 * (y_temp));
    *xy_sigma = -1 * *xy_sigma; // our axis are setup inverted in the y direction
}

/* Add in the mean of motion to the mean of robot_location */
void addMotion(gaussian_location_t* motion) {

/* Inverse the input to normal coordinates */
    // theta1 = 2*pi - theta1;
    robot_location.theta_mu = TWO_PI - robot_location.theta_mu;
    // theta2 = 2*pi - theta2;
    motion->theta_mu = TWO_PI - motion->theta_mu;
    // y2 = -1 * y2;
    motion->y_mu = -1 * motion->y_mu;


/* Calculate the change in x and y from rotating by the global theta */
    // x_change = x2 .* cos(theta1) - y2 .* sin(theta1);
    double x_delta = (motion->x_mu * cos(robot_location.theta_mu))
                        - (motion->y_mu * sin(robot_location.theta_mu));
    // y_change = x2 .* sin(theta1) + y2 .* cos(theta1);
    double y_delta = (motion->x_mu * sin(robot_location.theta_mu))
                        + (motion->y_mu * cos(robot_location.theta_mu));
    // y_change = -1 .* y_change;
    y_delta = -1 * y_delta; // Account for inverse y axis
    
/* Calculate final x and y location */
    // xf = x1 + x_change;
    robot_location.x_mu = robot_location.x_mu + x_delta;
    // yf = y1 + y_change;
    robot_location.y_mu = robot_location.y_mu + y_delta;

 /* Calculate the final theta direction */
    // thetaf = theta1 + theta2;
    robot_location.theta_mu = robot_location.theta_mu + motion->theta_mu;


/* Inverse final theta output */
    // thetaf = 2*pi - thetaf;
    robot_location.theta_mu = TWO_PI - robot_location.theta_mu;

/* limit final theta between 0 and 2 pi */

    while (robot_location.theta_mu < 0) { robot_location.theta_mu += TWO_PI; }

    while (robot_location.theta_mu > TWO_PI) { robot_location.theta_mu -= TWO_PI; }

}

