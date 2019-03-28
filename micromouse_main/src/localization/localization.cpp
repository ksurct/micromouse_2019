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
    robot_location.x.mean = INIT_X_MEAN;
    robot_location.x.sigma2 = INIT_X_SIGMA2;
    robot_location.y.mean = INIT_Y_MEAN;
    robot_location.y.sigma2 = INIT_Y_SIGMA2;
    robot_location.theta.mean = INIT_THETA_MEAN;
    robot_location.theta.sigma2 = INIT_THETA_SIGMA2;
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

