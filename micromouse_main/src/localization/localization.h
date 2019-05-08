/* localization.h */


#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_


#include "../types.h"


// The current state of the maze
extern probabilistic_maze_t robot_maze_state;

// The current location of the robot
extern gaussian_location_t robot_location;


/*----------- Public Functions -----------*/

/* initialize localization
 * Initializes robot_maze_state and robot_location */
void initializeLocalization(void);

/* localize motion step 
 * During the motion step we need to predict where
 * we will be and add uncertainty to our location
 * proportional to the amount we are trying to move */
gaussian_location_t* localizeMotionStep(double left_distance, double right_distance);

/* maze mapping
 * Using the sensor data and the current location,
 * update the robot_maze_state */
void mazeMappingAndMeasureStep(sensor_reading_t* sensor_data);


/*----------- Private Functions -----------*/
void calculateMotion(gaussian_location_t* motion, double left_distance, double right_distance);
void rotateCovariance(double rotate_by, double* x_sigma, double* y_sigma, double* xy_sigma);
void addMotion(gaussian_location_t* current_location, gaussian_location_t* motion,
                    gaussian_location_t* final_location);


#endif //_LOCALIZATION_H_
