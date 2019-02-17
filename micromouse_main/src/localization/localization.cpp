/* localization.cpp */

#include "../types.h"
#include "../devices/sensors.h"

#include "localization.h"
#include "probabilistic_maze.h"


// Globals
probabilistic_maze_t robot_maze_state;
gaussian_location_t robot_location;


/* initialize localizaiton */
void initializeLocalization() {

    // Initialize robot_maze_state
    initalizeMaze(&robot_maze_state);

    // Initialize robot_location
    robot_location.x.mean = INIT_X_MEAN;
    robot_location.x.sigma2 = INIT_X_SIGMA2;
    robot_location.y.mean = INIT_Y_MEAN;
    robot_location.y.sigma2 = INIT_Y_SIGMA2;
    robot_location.theta.mean = INIT_THETA_MEAN;
    robot_location.theta.sigma2 = INIT_THETA_SIGMA2;
}

/* localize measure step */
gaussian_location_t* localize_measure_step(sensor_t* sensor_data, double left_distance, double right_distance) {
    // dothething();
}

probabilistic_maze_t* mazeMapping(sensor_t* sensor_data) {
    // dothething();
}

/* localize motion step */
gaussian_location_t* localize_motion_step(double left_distance, double right_distance) {
    // dothething();
}

