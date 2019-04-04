/* settings.h */

#ifndef _SETTINGS_H_
#define _SETTINGS_H_


// General
#define DEBUG false

// Main
#define MAIN_LOOP_TIME 500000 // Delay between the start of each main_loop call in microseconds

// Maze Specifications
#define MAZE_SIZE       16          // If square, the length each side of the maze
#define MAZE_WIDTH      MAZE_SIZE   // Number of cells wide
#define MAZE_HEIGHT     MAZE_SIZE   // Number of cells tall
#define WALL_THICKNESS  12          // Thickness of the walls in mm
#define CELL_LENGTH     168         // Length and width of each cell inside the walls in mm

// Robot
#define WHEEL_RADIUS            16          // Wheel radius in mm
#define TICKS_PER_REVOLUTION    3575.0855   // Number to encoder ticks per one revolution of a wheel

// Localization
#define INIT_X_MEAN         100.0f  //dummy value (in mm)
#define INIT_X_SIGMA2       10.0f   //dummy value (in mm)
#define INIT_Y_MEAN         100.0f  //dummy value (in mm)
#define INIT_Y_SIGMA2       10.0f   //dummy value (in mm)
#define INIT_THETA_MEAN     0.0f    //dummy value (in mm)
#define INIT_THETA_SIGMA2   10.0f   //dummy value (in mm)

// Strategy
#define INIT_CELL_X     0       // Initial Cell x coordinate
#define INIT_CELL_Y     0       // Initial Cell y coordinate
#define GOAL_CELL_X     7       // Goal cell x coordinate
#define GOAL_CELL_Y     7       // Goal cell y coordinate
#define WALL_THRESHOLD  0.7     // Probability that we believe that a wall actually exists
#define MAX_VALUE       999   // Maximum value that can be in values

// Control
#define CONTROL_LOOP_TIME   5000    // Delay between start times of control loop in microseconds
#define TAU_P               0.05    // Proportional Gain
#define TAU_I               0.001   // Integral Gain
#define INT_BOUND           100     // Integral Bound

// Encoders
#define LEFT_ENCODER_PIN_A  48
#define LEFT_ENCODER_PIN_B  49
#define RIGHT_ENCODER_PIN_A 38
#define RIGHT_ENCODER_PIN_B 40

// Motors
#define LEFT    0
#define RIGHT   1
#define RESOLUTION_BITS     8       // Number of bits to use in analogWrite()
#define MAX_PWM_OUTPUT      255     // Should be 2^(RESOLUTION_BITS) - 1
#define MIN_PWM_OUTPUT      50      // Lowest PWM value that the motors can handle
#define LEFT_MOTOR_PIN_A    3       //dummy value
#define LEFT_MOTOR_PIN_B    6       //dummy value
#define RIGHT_MOTOR_PIN_A   4       //dummy value
#define RIGHT_MOTOR_PIN_B   5       //dummy value

// Sensors
#define I2C_RESET_PIN   3       //dummy value
#define TCAADDR         0x70
#define NUM_SENSORS     5
#define SENSOR_0_ADDR   0x00    //dummy value
#define SENSOR_0_PIN    0       //dummy value
#define SENSOR_1_ADDR   0x01    //dummy value
#define SENSOR_1_PIN    1       //dummy value
#define SENSOR_2_ADDR   0x02    //dummy value
#define SENSOR_2_PIN    2       //dummy value
#define SENSOR_3_ADDR   0x03    //dummy value
#define SENSOR_3_PIN    3       //dummy value
#define SENSOR_4_ADDR   0x04    //dummy value
#define SENSOR_4_PIN    4       //dummy value
#define SENSOR_5_ADDR   0x05    //dummy value
#define SENSOR_5_PIN    5       //dummy value

#define PI 3.14159265358979323846264

#endif //_SETTINGS_H_
