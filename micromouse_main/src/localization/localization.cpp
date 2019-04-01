/* localization.cpp */


#include "localization.h"
#include "../types.h"


// Globals
probabilistic_maze_t robot_maze_state;
gaussian_location_t robot_location;


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

/* localize motion step */
gaussian_location_t* localize_motion_step(double left_distance, double right_distance) {
    
    // Calculate the motion mean and covariance matrix
    
    // Update the current location and covariance matrix based on the distance travelled

}

/* localize measure step */
gaussian_location_t* localize_measure_step(sensor_reading_t* sensor_data) {
    // dothething();
}

probabilistic_maze_t* mazeMapping(sensor_reading_t* sensor_data) {
    // dothething();
}

