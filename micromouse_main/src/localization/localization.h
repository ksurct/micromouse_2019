/* localization.h */


#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_


#include "../types.h"
#include "../devices/sensors.h"

#include "probabilistic_maze.h"

// The current state of the maze
extern probabilistic_maze_t robot_maze_state;

// The current location of the robot
extern gaussian_location_t robot_location;

/* initialize localization
 * Initializes robot_maze_state and robot_location */
void initializeLocalization(void);

/* localize measure step
 * Using the data from the sensors, our old location
 * and the state of the maze, determine our current
 * location and update robot_location */
gaussian_location_t* localizeMeasureStep(sensor_reading_t* sensor_data, double left_distance, double right_distance);

/* maze mapping
 * Using the sensor data and the current location,
 * update the robot_maze_state */
probabilistic_maze_t* mazeMapping(sensor_reading_t* sensor_data);

/* localize motion step 
 * During the motion step we need to predict where
 * we will be and add uncertainty to our location
 * proportional to the amount we are trying to move */
gaussian_location_t* localizeMotionStep(double left_distance, double right_distance);

#endif //_LOCALIZATION_H_
