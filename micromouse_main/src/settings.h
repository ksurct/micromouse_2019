/* settings.h */

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// General
#define DEBUG false

// Main
#define MAIN_LOOP_TIME 10000 // Delay between the start of each main_loop call in microseconds

// Maze
#define MAZE_WIDTH  16
#define MAZE_HEIGHT 16

// Localization
#define INIT_X_MEAN         100.0   //dummy value
#define INIT_X_SIGMA2       10.0    //dummy value
#define INIT_Y_MEAN         100.0   //dummy value
#define INIT_Y_SIGMA2       10.0    //dummy value
#define INIT_THETA_MEAN     90.0    //dummy value
#define INIT_THETA_SIGMA2   10.0    //dummy value

// Motors
#define LEFT    0
#define RIGHT   1

// Sensors
#define I2C_RESET_PIN   3 //dummy value
#define NUM_SENSORS     5
#define TCAADDR         0x70

#endif //_SETTINGS_H_
