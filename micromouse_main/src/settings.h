/* settings.h */

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// General
#define DEBUG false

// Main
#define MAIN_LOOP_TIME 10000 // Delay between the start of each main_loop call in microseconds

// Maze Specifications
#define MAZE_WIDTH      16      // Number of cells wide
#define MAZE_HEIGHT     16      // Number of cells tall
#define WALL_THICKNESS  12      // Thickness of the walls in mm
#define CELL_LENGTH     168     // Length and width of each cell inside the walls in mm

// Localization
#define INIT_X_MEAN         100.0   //dummy value (in mm)
#define INIT_X_SIGMA2       10.0    //dummy value (in mm)
#define INIT_Y_MEAN         100.0   //dummy value (in mm)
#define INIT_Y_SIGMA2       10.0    //dummy value (in mm)
#define INIT_THETA_MEAN     0.0     //dummy value (in mm)
#define INIT_THETA_SIGMA2   10.0    //dummy value (in mm)

// Strategy
#define INIT_CELL_X     0       // Initial Cell x coordinate
#define INIT_CELL_Y     0       // Initial Cell y coordinate
#define GOAL_CELL_X     7       // Goal cell x coordinate
#define GOAL_CELL_Y     7       // Goal cell y coordinate
#define WALL_THRESHOLD  0.7     // Probability that we believe that a wall actually exists
#define MAX_VALUE       32767   // Maximum value that can be in values

// Motors
#define LEFT    0
#define RIGHT   1

// Sensors
#define I2C_RESET_PIN   3 //dummy value
#define NUM_SENSORS     5
#define TCAADDR         0x70

#endif //_SETTINGS_H_
