/* localization.cpp */


#include <math.h>

#include "localization.h"
#include "../settings.h"
#include "../types.h"
#include "../abs.h"
#include "../util/conversions.h"

// Temp
#include <stdio.h>


#define ENCODER_VARIANCE(d) (((ENCODER_VARIANCE_PER_MM) * abs(d)) + (ENCODER_VARIANCE_BASE))


// Globals
probabilistic_maze_t robot_maze_state;
gaussian_location_t robot_location;

/* Sensor offsets (Inverted y coordinates) */

gaussian_location_t sensor_offsets[NUM_SENSORS] = {
    { .x_mu = -32.5,  .y_mu = -35.0,  .theta_mu = -PI/2   },
    { .x_mu = 32.5,   .y_mu = -35.0,  .theta_mu = -PI/2   },
    { .x_mu = 46.0,   .y_mu = 0.0,    .theta_mu = 0.0     },
    { .x_mu = 32.5,   .y_mu = 35.0,   .theta_mu = PI/2    },
    { .x_mu = -32.5,  .y_mu = 35.0,   .theta_mu = PI/2    }
};

// Private Function Declarations
bool validateMeasurement(sensor_reading_t *measurement);
void processMeasurement(gaussian_location_t* location, sensor_reading_t *measurement);
void updateMazeWall(probabilistic_wall_t* wall);


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
    addMotion(&robot_location, &motion, &robot_location);

    return &robot_location;
}

/* Maze Mapping
 * - Updates the global robot_maze_state based on the believed current position and the sensor data recorded */
probabilistic_maze_t* mazeMapping(sensor_reading_t* sensor_data) {
    
    printf("Robot:   \t(%f,\t%f,\t%f)\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);
    
    // calculate the sensor locations
    gaussian_location_t sensor_locations[NUM_SENSORS];
    for (int i = 0; i < NUM_SENSORS; i++) {
        
        if (validateMeasurement(&sensor_data[i])) {
            
            addMotion(&robot_location, &sensor_offsets[i], &sensor_locations[i]);
            printf("Sensor %d:\t(%f,\t%f,\t%f)\n", i, sensor_locations[i].x_mu, sensor_locations[i].y_mu, sensor_locations[i].theta_mu);

            processMeasurement(&sensor_locations[i], &(sensor_data[i]));
        }
    }

    return &robot_maze_state;
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

/* Add in the mean of motion to the mean of current_location to get the final location */
void addMotion(gaussian_location_t* current_location, gaussian_location_t* motion,
                    gaussian_location_t* final_location) {

/* Inverse the input to normal coordinates */
    // theta1 = 2*pi - theta1;
    final_location->theta_mu = TWO_PI - current_location->theta_mu;
    // theta2 = 2*pi - theta2;
    motion->theta_mu = TWO_PI - motion->theta_mu;
    // y2 = -1 * y2;
    motion->y_mu = -1 * motion->y_mu;


/* Calculate the change in x and y from rotating by the global theta */
    // x_change = x2 .* cos(theta1) - y2 .* sin(theta1);
    double x_delta = (motion->x_mu * cos(final_location->theta_mu))
                        - (motion->y_mu * sin(final_location->theta_mu));
    // y_change = x2 .* sin(theta1) + y2 .* cos(theta1);
    double y_delta = (motion->x_mu * sin(final_location->theta_mu))
                        + (motion->y_mu * cos(final_location->theta_mu));
    // y_change = -1 .* y_change;
    y_delta = -1 * y_delta; // Account for inverse y axis
    
/* Calculate final x and y location */
    // xf = x1 + x_change;
    final_location->x_mu = current_location->x_mu + x_delta;
    // yf = y1 + y_change;
    final_location->y_mu = current_location->y_mu + y_delta;

 /* Calculate the final theta direction */
    // thetaf = theta1 + theta2;
    final_location->theta_mu = final_location->theta_mu + motion->theta_mu;


/* Inverse final theta output */
    // thetaf = 2*pi - thetaf;
    final_location->theta_mu = TWO_PI - final_location->theta_mu;

/* limit final theta between 0 and 2 pi */
    while (final_location->theta_mu < 0) { final_location->theta_mu += TWO_PI; }
    while (final_location->theta_mu >= TWO_PI) { final_location->theta_mu -= TWO_PI; }

}

// Check if we should process measurement
bool validateMeasurement(sensor_reading_t *measurement) {
    return (measurement->state != ERROR && measurement->state != WAITING);
}

/* update robot_maze_state based on the given sensor reading */
void processMeasurement(gaussian_location_t* location, sensor_reading_t *measurement) {
    
    // Ray cast to find places where we hit a wall and update each wall we hit

    //which box of the map we're in
    int cellX = coordinateDistanceToCellNumber(location->x_mu);
    int cellY = coordinateDistanceToCellNumber(location->y_mu);
    double edgeCellX = cellNumberToCoordinateDistance(cellX) - (CELL_LENGTH + WALL_THICKNESS) / 2;
    double edgeCellY = cellNumberToCoordinateDistance(cellY) - (CELL_LENGTH + WALL_THICKNESS) / 2;

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

    //the direction of the ray as a vector
    double rayDirX = cos(location->theta_mu);
    double rayDirY = sin(location->theta_mu);

    //length of ray from one x or y-side to next x or y-side
    double deltaDistX = abs(1 / rayDirX) * (CELL_LENGTH + WALL_THICKNESS);
    double deltaDistY = abs(1 / rayDirY) * (CELL_LENGTH + WALL_THICKNESS);
    printf("deltaDist:\t(%f,\t%f)\n", deltaDistX, deltaDistY);

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    //calculate step and initial sideDist
    double percent; // temp variable
    if (rayDirX < 0) {
        stepX = -1;
        percent = (location->x_mu - edgeCellX) / (CELL_LENGTH + WALL_THICKNESS);
        sideDistX = percent * deltaDistX;
    } else {
        stepX = 1;
        percent = (edgeCellX + (CELL_LENGTH + WALL_THICKNESS) - location->x_mu) / (CELL_LENGTH + WALL_THICKNESS);
        sideDistX = percent * deltaDistX;
    }
    
    if (rayDirY < 0) {
        stepY = -1;
        percent = (location->y_mu - edgeCellY) / (CELL_LENGTH + WALL_THICKNESS);
        sideDistY = percent * deltaDistY;
    } else {
        stepY = 1;
        percent = (edgeCellY + (CELL_LENGTH + WALL_THICKNESS) - location->y_mu) / (CELL_LENGTH + WALL_THICKNESS);
        sideDistY = percent * deltaDistY;
    }
    printf("sideDist:\t(%f,\t%f)\n", sideDistX, sideDistY);
    printf("step:\t\t(%d,\t%d)\n", stepX, stepY);

    printf("\n");
    printf("State: %d, Distance: %d\n", measurement->state, measurement->distance);

    // Traverse the for length of the measurement and still within the maze
    while ( (sideDistX < measurement->distance || sideDistY < measurement->distance)
                && !(cellX < 0 || cellX >= (MAZE_WIDTH) || cellY < 0 || cellY >= (MAZE_HEIGHT))) {
        
        //jump to next map square, in x-direction, OR in y-direction
        if (sideDistX < sideDistY) { // x-direction
            
            sideDistX += deltaDistX; // Set to next x collision distance

            if (stepX > 0) { // Update East and go East
                printf("%d %d east\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].east);
                cellX += stepX;
            } else { // Update West and go West
                printf("%d %d west\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].west);
                cellX += stepX;
            }
        } else { // y-direction
            
            sideDistY += deltaDistY; // Set to next y collision distance
            
            if (stepY > 0) { // Update South and go South
                printf("%d %d south\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].south);
                cellY += stepY;
            } else { // Update North and go North
                printf("%d %d north\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].north);
                cellY += stepY;
            }
        }
    } 

    printf("\n");
}


// Raycast has hit this wall, update it accordingly
void updateMazeWall(probabilistic_wall_t* wall) {

}
