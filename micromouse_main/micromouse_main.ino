/* Micromouse 2019 */

// Libraries
#include <DueTimer.h>

// General
#include "src/settings.h"
#include "src/types.h"

// Devices
#include "src/devices/encoders.h"
#include "src/devices/sensors.h"
//#include "src/devices/motors.h"

// Subsystems
#include "src/localization/localization.h"
#include "src/localization/probabilistic_maze.h"

#include "src/control/control.h"

// Utilities
#include "src/util/conversions.h"

// Function Declarations
void main_loop(void);
void setup(void);


/* Entry point to the code for the robot, all initialization
 * of subsystems should be done here */
void setup() {

  // Initialize serial if in debug mode
  if (DEBUG) {
    Serial.begin(9600);
  }

  // Setup Sensors
  if (! sensorSetup()){
    // Throw error? Serial message maybe?
  }
  
  // Setup Encoders
  encoderSetup(LEFT, LEFT_ENCODER_PIN_A, LEFT_ENCODER_PIN_B);
  encoderSetup(RIGHT, RIGHT_ENCODER_PIN_A, RIGHT_ENCODER_PIN_B);
  
  // Initialize Localization subsystem
  initializeLocalization();

  // Initialize Strategy subsystem
  //initializeStrategy();

  // Initialize Control subsystem (includes motors)
  initializeControl();

  // Start main loop
  Timer0.attachInterrupt(main_loop);
  Timer0.start(MAIN_LOOP_TIME);
}


void main_loop() {

  // Initalize variables
  static sensor_t sensor_data[NUM_SENSORS];
  static double left_distance;
  static double right_distance;
  static double left_speed;
  static double right_speed;

  // Get sensor data
  if (!readSensors(sensor_data)){
    // throw error and log to serial
  }

  // Get distance travelled from control subsystem
  distanceTravelled(&left_distance, &right_distance);

  // Interpolate sensor and encoder data together using a kalman filter (measurement step)
  //localizeMeasureStep(sensor_data, left_distance, right_distance);

  // Update maze with sensor readings
  //mazeMapping(sensor_data);

  // Determine next cell to go to (strategy step)
  //gaussian_location_t* next_location = strategy(&robot_location, &robot_maze_state);
  
  // Determine what speed to set the motors to (speed profile + error correction, or turning profile + error correction)
  //calculateSpeed(current_location, next_location, &left_speed, &right_speed);

  // Run predictions through the kalman filter (motion step)
  //localizeMotionStep(left_speed * MAIN_LOOP_TIME, right_speed * MAIN_LOOP_TIME);

  // Set speed using the motor controllers (pid loop)
  //setSpeedPID(left_speed, right_speed);
}

/* This function is not in use because we would like to control the timing that the loop is called */
void loop(){
  // do not use
}
