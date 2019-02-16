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

// Robot
#define WHEEL_RADIUS            16          // Wheel radius in mm
#define TICKS_PER_REVOLUTION    3575.0855   // Number to encoder ticks per one revolution of a wheel

// Localization
#define INIT_X_MEAN         100.0   //dummy value
#define INIT_X_SIGMA2       10.0    //dummy value
#define INIT_Y_MEAN         100.0   //dummy value
#define INIT_Y_SIGMA2       10.0    //dummy value
#define INIT_THETA_MEAN     90.0    //dummy value
#define INIT_THETA_SIGMA2   10.0    //dummy value

// Control
#define CONTROL_LOOP_TIME   1000    // Delay between start times of control loop in microseconds
#define TAU_P               1000    //dummy value
#define TAU_I               100     //dummy value

// Motors
#define LEFT    0
#define RIGHT   1
#define RESOLUTION_BITS     12      // Number of bits to use in analogWrite()
#define MAX_PWM_OUTPUT      4095    // Should be 2^(RESOLUTION_BITS) - 1
#define LEFT_MOTOR_PIN_A    1       //dummy value
#define LEFT_MOTOR_PIN_B    1       //dummy value
#define RIGHT_MOTOR_PIN_A   1       //dummy value
#define RIGHT_MOTOR_PIN_B   1       //dummy value

// Sensors
#define I2C_RESET_PIN   3 //dummy value
#define NUM_SENSORS     5
#define TCAADDR         0x70

#endif //_SETTINGS_H_
