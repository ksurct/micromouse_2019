/* localization.cpp */


#include <Arduino.h>
#include <math.h>

#include "localization.h"
#include "../types.h"


#define ENCODER_VARIANCE(d) (((ENCODER_VARIANCE_PER_MM) * abs(d)) + (ENCODER_VARIANCE_BASE))


// Globals
probabilistic_maze_t robot_maze_state;
gaussian_location_t robot_location;


// Private Functions
void calculateMotion(gaussian_location_t* motion, double left_distance, double right_distance);
void addMotion(gaussian_location_t* motion);


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

void calculateMotion(gaussian_location_t* motion, double left_distance, double right_distance) {

    /* Run the motion through the system model */
    
    // If going straight
    if (left_distance == right_distance) {
        
        motion->x_mu = left_distance;
        motion->y_mu = 0.0;
        motion->theta_mu = 0.0;
    } else {
        // R = (l/2)*((dr + dl)./(dr - dl));
        double turn_radius = (WHEEL_BASE_LENGTH / 2) * ((right_distance + left_distance) / (right_distance - left_distance));

        // change_in_theta = (dr - dl)/l;
        double delta_theta = (right_distance - left_distance) / WHEEL_BASE_LENGTH;

        // ICC = [x - R.*sin(theta), y + R.*cos(theta)];

        // xf = ((x - ICC(:,1)) .* cos(change_in_theta) - (y - ICC(:,2)) .* sin(change_in_theta)) + ICC(:,1);
        motion->x_mu = turn_radius * sin(delta_theta);

        // yf = ((x - ICC(:,1)) .* sin(change_in_theta) + (y - ICC(:,2)) .* cos(change_in_theta)) + ICC(:,2);
        motion->y_mu = -1 * turn_radius * cos(delta_theta) + turn_radius;

        // thetaf = theta + change_in_theta;
        motion->theta_mu = delta_theta;
    }

    
    /* Calculate the change in the covariance matrix */
    
    double distance_travelled = (left_distance + right_distance) / 2;

    motion->x_sigma = X_VARIANCE * ENCODER_VARIANCE(distance_travelled);
    motion->y_sigma = Y_VARIANCE * ENCODER_VARIANCE(distance_travelled);
    motion->theta_sigma = THETA_VARIANCE * ENCODER_VARIANCE(distance_travelled);

    
    
}

void addMotion(gaussian_location_t* motion) {
    
}
