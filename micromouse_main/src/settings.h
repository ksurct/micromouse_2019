/* settings.h */

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// General
#define DEBUG false;

// Main
#define MAIN_LOOP_TIME 10000 // Delay between the start of each main_loop call in nanoseconds

// Maze
#define MAZE_WIDTH 16
#define MAZE_HEIGHT 16

// Motors
#define LEFT 0
#define RIGHT 1

// Sensors
#define I2C_RESET_PIN 3 //dummy value
#define NUM_SENSORS 5
#define TCAADDR 0x70

#endif //_SETTINGS_H_
