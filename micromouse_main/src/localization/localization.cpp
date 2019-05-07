/* localization.cpp */


#include <math.h>

#include "localization.h"
#include "../settings.h"
#include "../types.h"
#include "../abs.h"
#include "../util/conversions.h"

// Temp
#include <stdio.h>


typedef struct {
    bool hit;               // Does the wall we hit exists?
    double distance_hit;    // What was the furthest distance from the sensor to the hit
    char side;              // Determines the orientation of the wall we hit, 0: parallel to x-axis, 1: parallel to y-axis
    Direction dir;          // The direction from the robot to the wall hit
    probabilistic_wall_t *wall;
} hit_data_t;


#define ENCODER_VARIANCE(d) (((ENCODER_VARIANCE_PER_MM) * abs(d)) + (ENCODER_VARIANCE_BASE))


// Globals
probabilistic_maze_t robot_maze_state;
gaussian_location_t robot_location;

/* Sensor offsets (Inverted y coordinates) */

gaussian_location_t sensor_offsets[NUM_SENSORS] = {
    { .x_mu = -SENSOR_X_OFFSET,     .y_mu = -SENSOR_Y_OFFSET,   .theta_mu = -PI/2   },    // Sensor 0 (Bottom left)
    { .x_mu = SENSOR_X_OFFSET,      .y_mu = -SENSOR_Y_OFFSET,   .theta_mu = -PI/2   },    // Sensor 1 (Top left)
    { .x_mu = SENSOR_FRONT_OFFSET,  .y_mu = 0.0,                .theta_mu = 0.0     },    // Sensor 2 (Front)
    { .x_mu = SENSOR_X_OFFSET,      .y_mu = SENSOR_Y_OFFSET,    .theta_mu = PI/2    },    // Sensor 3 (Top right)
    { .x_mu = -SENSOR_X_OFFSET,     .y_mu = SENSOR_Y_OFFSET,    .theta_mu = PI/2    }     // Sensor 4 (Bottom right)
};

// Private Function Declarations
bool validateMeasurement(sensor_reading_t *measurement);
void processMeasurementMapping(gaussian_location_t* location, sensor_reading_t *measurement, hit_data_t* hit_data, int sensor_num);
void updateMazeWall(probabilistic_wall_t* wall, double distance_hit, sensor_reading_t * reading, int sensor_num);
void processMeasurementMeasure(gaussian_location_t* sensor_location, sensor_reading_t* measurement,
                                    hit_data_t* hit_data, gaussian_location_t* new_location);


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
void mazeMappingAndMeasureStep(sensor_reading_t* sensor_data) {
    
    // printf("Robot:   \t(%f,\t%f,\t%f)\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);
    
/* Update the maze based on the sensor_data */

    gaussian_location_t sensor_locations[NUM_SENSORS];
    hit_data_t sensor_hit_data[NUM_SENSORS];
    for (int i = 0; i < NUM_SENSORS; i++) {
        
        addMotion(&robot_location, &sensor_offsets[i], &sensor_locations[i]);
        // printf("Sensor %d:\t(%f,\t%f,\t%f)\n", i, sensor_locations[i].x_mu, sensor_locations[i].y_mu, sensor_locations[i].theta_mu);

        if (validateMeasurement(&sensor_data[i])) {
            processMeasurementMapping(&sensor_locations[i], &sensor_data[i], &sensor_hit_data[i], i);
        }
    }


/* Update the robot's location based on the sensor_data and the new maze */

    gaussian_location_t sensor_location = {
        .x_mu = 0.0,
        .y_mu = 0.0,
        .theta_mu = 0.0,
        .x_sigma = 0.0,
        .xy_sigma = 0.0,
        .y_sigma = 0.0,
        .theta_sigma = 0.0
    };
    
    // just use the sensor data and average out x and y location
    double sumX = 0.0; double sumY = 0.0;
    double tempX = 0.0; double tempY = 0.0;
    char count = 0;
    for (int i = 0; i < NUM_SENSORS; i++) {
        // printf("Sensor: %d, ", i);
        // Don't update robot_location on anything thats not good or the wall doesn't exists
        if (sensor_data[i].state == GOOD && sensor_hit_data[i].hit) {

            double shift_forward = sensor_hit_data[i].distance_hit - sensor_data[i].distance;
            // printf("shift_forward: %f, ", shift_forsward);

            // Shift the robot_location by shift_forward in the direction of the sensor's location and add to sum
            sumX += (shift_forward * cos(sensor_locations[i].theta_mu)) + robot_location.x_mu;
            sumY += (shift_forward * sin(sensor_locations[i].theta_mu)) + robot_location.y_mu;
            // printf("sensor_locations[%d]: ( %f, %f )\n", i, (shift_forward * cos(sensor_locations[i].theta_mu)) + robot_location.x_mu, (shift_forward * sin(sensor_locations[i].theta_mu)) + robot_location.y_mu);
            // Serial.print("sensor_locations[");
            // Serial.print(i);
            // Serial.print("]: ( ");
            // Serial.print((shift_forward * cos(sensor_locations[i].theta_mu)) + robot_location.x_mu);
            // Serial.print(", ");
            // Serial.print((shift_forward * sin(sensor_locations[i].theta_mu)) + robot_location.y_mu);
            // Serial.println(" )");
            // printf("Sums: ( %f, %f )", sumX, sumY);

            count++;
        }
    }
    // printf("\n");

    // average the x and y values
    if (count > 0) {
        sensor_location.x_mu = (sumX / count);
        sensor_location.y_mu = (sumY / count);
    } else {
        sensor_location.x_mu = robot_location.x_mu;
        sensor_location.y_mu = robot_location.y_mu;
    }

    // printf("sensor_location: (%f, %f)\n", sensor_location.x_mu, sensor_location.y_mu);
    // Serial.print("sensor_location: ( ");
    // Serial.print(sensor_location.x_mu);
    // Serial.print(", ");
    // Serial.print(sensor_location.y_mu);
    // Serial.println(" )");

    // Perform a weighted average between the new location and the old location
    robot_location.x_mu = (SENSOR_LOCATION_WEIGHT * sensor_location.x_mu) + ((1 - SENSOR_LOCATION_WEIGHT) * robot_location.x_mu);
    robot_location.y_mu = (SENSOR_LOCATION_WEIGHT * sensor_location.y_mu) + ((1 - SENSOR_LOCATION_WEIGHT) * robot_location.y_mu);

    // Create a reading for theta during special circumstances(such as both readings on one side hitting a wall)
    char sensor_theta_count = 0;
    double relative_forward;
    // both left side sensors
    if (sensor_data[0].state == GOOD && sensor_data[1].state == GOOD
        && sensor_hit_data[0].hit && sensor_hit_data[1].hit
        && sensor_hit_data[0].dir == sensor_hit_data[1].dir) {

        sensor_theta_count++;
        
        if (sensor_hit_data[0].dir == North) {
            relative_forward = 0.0;
        } else {
            relative_forward = directionToRAD[sensor_hit_data[0].dir] + PI/2;
        }
        // printf("relative_forward: %f\n", relative_forward);
        sensor_location.theta_mu += relative_forward + tan( (sensor_data[0].distance - sensor_data[1].distance) / (SENSOR_X_OFFSET * 2));
        // printf("Thing 1: %f\n", relative_forward + tan( (sensor_data[0].distance - sensor_data[1].distance) / (SENSOR_X_OFFSET * 2)));
    }

    // both right side sensors
    if (sensor_data[3].state == GOOD && sensor_data[4].state == GOOD
        && sensor_hit_data[3].hit && sensor_hit_data[4].hit
        && sensor_hit_data[3].dir == sensor_hit_data[4].dir) {
        
        sensor_theta_count++;
                
        if (sensor_hit_data[3].dir == East)
            relative_forward = 3*PI/2;
        else
            relative_forward = directionToRAD[sensor_hit_data[3].dir] - PI/2;
        // printf("relative_forward: %f\n", relative_forward);
        sensor_location.theta_mu += relative_forward + tan( (sensor_data[4].distance - sensor_data[3].distance) / (SENSOR_X_OFFSET * 2));
        // printf("Thing 2: %f\n", relative_forward + tan( (sensor_data[4].distance - sensor_data[3].distance) / (SENSOR_X_OFFSET * 2)));
    }


    // both top sensors
    if (sensor_data[1].state == GOOD && sensor_data[3].state == GOOD && 
        sensor_hit_data[1].side == sensor_hit_data[3].side) {
        // TODO
    }


    // both bottom sensors
    if (sensor_data[0].state == GOOD && sensor_data[4].state == GOOD &&
        sensor_hit_data[0].side == sensor_hit_data[4].side) {
        // TODO
    }


    // If sensor reading of theta, use it
    if (sensor_location.theta_mu != 0.0) {
        
        sensor_location.theta_mu = (sensor_location.theta_mu / sensor_theta_count);
        
        // printf("sensor_location.theta_mu: %f\n", sensor_location.theta_mu);

        /* limit sensor theta between 0 and 2 pi */
        while (sensor_location.theta_mu < 0) { sensor_location.theta_mu += TWO_PI; }
        while (sensor_location.theta_mu >= TWO_PI) { sensor_location.theta_mu -= TWO_PI; }

        double delta = sensor_location.theta_mu - robot_location.theta_mu;
        if (-PI <= delta && delta <= PI) {
            robot_location.theta_mu += (SENSOR_LOCATION_WEIGHT * delta) + ((1 - SENSOR_LOCATION_WEIGHT) * 0.0);
        } else if (-TWO_PI <= delta && delta < -PI) {
            robot_location.theta_mu += (SENSOR_LOCATION_WEIGHT * delta) + ((1 - SENSOR_LOCATION_WEIGHT) * -TWO_PI);
        } else if (PI < delta && delta <= TWO_PI) {
            robot_location.theta_mu += (SENSOR_LOCATION_WEIGHT * delta) + ((1 - SENSOR_LOCATION_WEIGHT) * TWO_PI);
        } else {
            // printf("ERROR ERROR ERROR\n");
            //Serial.println("ERROR ERROR ERROR");
        }

        //robot_location.theta_mu = (SENSOR_LOCATION_WEIGHT * sensor_location.theta_mu) + ((1 - SENSOR_LOCATION_WEIGHT) * robot_location.theta_mu);

        /* limit robot theta between 0 and 2 pi */
        while (robot_location.theta_mu < 0) { robot_location.theta_mu += TWO_PI; }
        while (robot_location.theta_mu >= TWO_PI) { robot_location.theta_mu -= TWO_PI; }
    }

    // printf("robot_location.theta_mu: %f\n", robot_location.theta_mu);

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

/* reset motion vector to original value */
    // theta2 = 2*pi - theta2;
    motion->theta_mu = TWO_PI - motion->theta_mu;
    // y2 = -1 * y2;
    motion->y_mu = -1 * motion->y_mu;


}

// Check if we should process measurement
bool validateMeasurement(sensor_reading_t *measurement) {
    return (measurement->state != ERROR && measurement->state != WAITING);
}

/* update robot_maze_state based on the given sensor reading */
void processMeasurementMapping(gaussian_location_t* location, sensor_reading_t *measurement, hit_data_t* hit_data, int sensor_num) {
    
    // Use defaults for TOO_FAR and TOO_CLOSE STATES
    if (measurement->state == TOO_FAR)
        measurement->distance = TOO_FAR_DISTANCE;
    
    if (measurement->state == TOO_CLOSE)
        measurement->distance = TOO_CLOSE_DISTANCE;

    // Ray cast to find places where we hit a wall and update each wall we hit

    //which box of the map we're in
    int cellX = coordinateDistanceToCellNumber(location->x_mu);
    int cellY = coordinateDistanceToCellNumber(location->y_mu);
    double edgeCellX = cellNumberToCoordinateDistance(cellX) - (CELL_LENGTH) / 2;
    double edgeCellY = cellNumberToCoordinateDistance(cellY) - (CELL_LENGTH) / 2;

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

    //the direction of the ray as a vector
    double rayDirX = cos(location->theta_mu);
    double rayDirY = sin(location->theta_mu);

    //length of ray from one x or y-side to next x or y-side
    double deltaDistX = abs(1 / rayDirX) * (CELL_LENGTH + WALL_THICKNESS);
    double deltaDistY = abs(1 / rayDirY) * (CELL_LENGTH + WALL_THICKNESS);

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
        percent = (edgeCellX + (CELL_LENGTH) - location->x_mu) / (CELL_LENGTH + WALL_THICKNESS);
        sideDistX = percent * deltaDistX;
    }
    
    if (rayDirY < 0) {
        stepY = -1;
        percent = (location->y_mu - edgeCellY) / (CELL_LENGTH + WALL_THICKNESS);
        sideDistY = percent * deltaDistY;
    } else {
        stepY = 1;
        percent = (edgeCellY + (CELL_LENGTH) - location->y_mu) / (CELL_LENGTH + WALL_THICKNESS);
        sideDistY = percent * deltaDistY;
    }

    // printf("sideDist(X,Y): (%f, %f)\n", sideDistX, sideDistY);
    // printf("step(X,Y): (%d, %d)\n", stepX, stepY);

    // In case we don't reach a wall
    if (sideDistX < sideDistY) {
        // Save this hit as the farthest and side as the x axis
        hit_data->distance_hit = sideDistX;
        hit_data->side = 0;
        if (stepX > 0) {
            hit_data->dir = East;
            hit_data->wall = robot_maze_state.cells[cellX][cellY].east;
        } else {
            hit_data->dir = West;
            hit_data->wall = robot_maze_state.cells[cellX][cellY].west;
        }
        
    } else {
        // Save this hit as the farthest and side as the y axis
        hit_data->distance_hit = sideDistY;
        hit_data->side = 1;
        if (stepY > 0) {
            hit_data->dir = South;
            hit_data->wall = robot_maze_state.cells[cellX][cellY].south;
        } else {
            hit_data->dir = North;
            hit_data->wall = robot_maze_state.cells[cellX][cellY].north;
        }
    }

    if (hit_data->wall->exists > WALL_THRESHOLD) {
        hit_data->hit = true;
    } else {
        hit_data->hit = false;
    }

    // Traverse the for length of the measurement + threshold and still within the maze
    while ( (sideDistX < measurement->distance + WALL_HIT_THRESHOLD || sideDistY < measurement->distance + WALL_HIT_THRESHOLD)
                && !(cellX < 0 || cellX >= (MAZE_WIDTH) || cellY < 0 || cellY >= (MAZE_HEIGHT))) {
        
        //jump to next map square, in x-direction, OR in y-direction
        if (sideDistX < sideDistY) { // x-direction

            if (stepX > 0) { // Update East and move East
                // printf("%d %d east\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].east, sideDistX, measurement, sensor_num);
                hit_data->wall = robot_maze_state.cells[cellX][cellY].east;
                hit_data->dir = East;
                cellX++;
            } else { // Update West and move West
                // printf("%d %d west\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].west, sideDistX, measurement, sensor_num);
                hit_data->wall = robot_maze_state.cells[cellX][cellY].west;
                hit_data->dir = West;
                cellX--;
            }
            
            // Save this hit as the farthest and side as the x axis
            hit_data->distance_hit = sideDistX;
            hit_data->side = 0;

            sideDistX += deltaDistX; // Set to next x collision distance
        } else { // y-direction
            
            if (stepY > 0) { // Update South and move South
                // printf("%d %d south\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].south, sideDistY, measurement, sensor_num);
                hit_data->wall = robot_maze_state.cells[cellX][cellY].south;
                hit_data->dir = South;
                cellY++;
            } else { // Update North and move North
                // printf("%d %d north\n", cellX, cellY);
                updateMazeWall(robot_maze_state.cells[cellX][cellY].north, sideDistY, measurement, sensor_num);
                hit_data->wall = robot_maze_state.cells[cellX][cellY].north;
                hit_data->dir = North;
                cellY--;
            }

            // Save this hit as the farthest and side as the y axis
            hit_data->distance_hit = sideDistY;
            hit_data->side = 1;

            sideDistY += deltaDistY; // Set to next y collision distance
        }

        if (hit_data->wall->exists > WALL_THRESHOLD) {
            hit_data->hit = true;
            break;
        } else {
            hit_data->hit = false;
        }
    }

    // printf("cell(X,Y): (%d, %d)\n", cellX, cellY);

    // printf("hit_data: \n");
    // printf("\thit: %d\n", hit_data->hit);
    // printf("\tdistance_hit: %f\n", hit_data->distance_hit);
    // printf("\tside: %d\n", hit_data->side);
    // printf("\tdir: %d\n", hit_data->dir);
    // printf("\twall: %x\n", hit_data->wall);

    // printf("\n");
}


/* Raycast has hit a wall at distance_hit away from the original sensor measurement
 * - Note: order of operations does matter for multiplicative but not for additive */
void updateMazeWall(probabilistic_wall_t* wall, double distance_hit, sensor_reading_t* measurement, int sensor_num) {

    // Additive implementation
    // if (measurement->distance - WALL_HIT_THRESHOLD < distance_hit &&
    //         distance_hit < measurement->distance + WALL_HIT_THRESHOLD) {
    //     // hit should increase wall value if state is GOOD or TOO_CLOSE
    //     if (measurement->state == GOOD || measurement->state == TOO_CLOSE)
    //         wall->exists += WALL_UPDATE_AMOUNT;
    // } else {
    //     // hit should decrease wall value if state is GOOD or TOO_FAR
    //     if (measurement->state == GOOD || measurement->state == TOO_FAR)
    //         wall->exists -= WALL_UPDATE_AMOUNT;
    // }

    // Multiplicative implementation
    if (measurement->distance - WALL_HIT_THRESHOLD < distance_hit &&
            distance_hit < measurement->distance + WALL_HIT_THRESHOLD) {
        // hit should increase wall value if state is GOOD or TOO_CLOSE
        if (measurement->state == GOOD || measurement->state == TOO_CLOSE) {
            wall->exists = (1.0 - ((1.0 - wall->exists) * WALL_UPDATE));
        }
    } else {
        // hit should decrease wall value if state is GOOD or TOO_FAR
        if (measurement->state == GOOD || measurement->state == TOO_FAR) {
            wall->exists = wall->exists * WALL_UPDATE;
        }
    }

    // bound the value of wall->exists by 1.0 and 0.0
    if (wall->exists > 1.0) wall->exists = 1.0;
    if (wall->exists < 0.0) wall->exists = 0.0;
}


void processMeasurementMeasure(gaussian_location_t* sensor_location, sensor_reading_t* measurement,
                                    hit_data_t* hit_data, gaussian_location_t* new_location) {
    // shift_forward = distance_hit-measurement
    double shift_forward = hit_data->distance_hit - measurement->distance;

    // Shift the robot_location by shift_forward in the direction of the sensor's location
    new_location->x_mu = shift_forward * cos(sensor_location->theta_mu) + robot_location.x_mu;
    new_location->y_mu = shift_forward * sin(sensor_location->theta_mu) + robot_location.y_mu;
    
    new_location->theta_mu = robot_location.theta_mu;
}
