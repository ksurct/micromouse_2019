/* Micromouse 2019 */


// Libraries
#include <DueTimer.h>

// General
#include "src/settings.h"
#include "src/types.h"

// Devices
#include "src/devices/sensors.h"
#include "src/devices/motors.h"
#include "src/devices/encoders.h"

// Subsystems
#include "src/localization/localization.h"
#include "src/strategy/strategy.h"
#include "src/movement/movement.h"
#include "src/control/control.h"

// Utilities
#include "src/util/conversions.h"


// Function Declarations
void setup(void);
void start_loop(void);
void main_loop(void);


/* Entry point to the code for the robot, all initialization
   of subsystems should be done here */
void setup() {

  // Initialize Serial
  Serial.begin(115200);

  // Setup Sensors
  if (! sensorSetup()) {
    Serial.println("Error connecting to sensors!");
  }

  // Setup Motors
  motorSetup();

  // Setup Encoders
  encoderSetup(LEFT, LEFT_ENCODER_PIN_A, LEFT_ENCODER_PIN_B);
  encoderSetup(RIGHT, RIGHT_ENCODER_PIN_A, RIGHT_ENCODER_PIN_B);

  // Initialize Localization subsystem
  initializeLocalization();

  // Initialize Strategy subsystem
  initializeStrategy();

  // Initialize Control subsystem
  initializeControl();

  // Wait 2 seconds
  delay(2000);

  Timer1.attachInterrupt(start_loop).start(MAIN_LOOP_TIME);

  // // Start start loop
  while (robot_location.theta_mu < 3*PI/2) {
    Serial.println("Test");
    Serial.println(robot_location.theta_mu);
  }
  Serial.println("Should have stopped");
  Timer1.stop();
  
  // // Start main loop
  Timer1.attachInterrupt(main_loop).start(MAIN_LOOP_TIME);
}

void start_loop() {

  // Initialize variables
  static sensor_reading_t sensor_data[NUM_SENSORS];
  static double left_distance;
  static double right_distance;
  static double left_speed;
  static double right_speed;
  //static gaussian_location_t next_location = {.x_mu = 264.0, .y_mu = 84.0}; -REMOVE

  // Get sensor data
  //readSensors(sensor_data);

  // Get distance travelled from control subsystem
  distanceTravelled(&left_distance, &right_distance);

  // Run distances through localization
  localizeMotionStep(left_distance, right_distance);

  // Update maze with sensor readings
  //mazeMapping(sensor_data);

  // Interpolate sensor data into the localization
  //localizeMeasureStep(sensor_data);

  // Determine next cell to go to (strategy step)
  //strategy(&robot_location, &robot_maze_state, &next_location); -REMOVE

  // Determine what speed to set the motors to (speed profile + error correction, or turning profile + error correction)
  //calculateSpeed(&robot_location, &next_location, &left_speed, &right_speed); -REMOVE

  // Set speed using the motor controllers (pid loop)
  setSpeedPID(TURN_PROFILE_STABLE_SPEED, -TURN_PROFILE_STABLE_SPEED);

}

void main_loop() {

  // Initialize variables
  static sensor_reading_t sensor_data[NUM_SENSORS];
  static double left_distance;
  static double right_distance;
  static double left_speed;
  static double right_speed;
  static gaussian_location_t next_location = {.x_mu = 264.0, .y_mu = 84.0};

  // Get sensor data
  //readSensors(sensor_data);

  // Get distance travelled from control subsystem
  distanceTravelled(&left_distance, &right_distance);

  // Run distances through localization
  localizeMotionStep(left_distance, right_distance);

  // Update maze with sensor readings
  //mazeMapping(sensor_data);

  // Interpolate sensor data into the localization
  //localizeMeasureStep(sensor_data);

  // Determine next cell to go to (strategy step)
  //strategy(&robot_location, &robot_maze_state, &next_location);

  // Determine what speed to set the motors to (speed profile + error correction, or turning profile + error correction)
  calculateSpeed(&robot_location, &next_location, &left_speed, &right_speed);

  // Set speed using the motor controllers (pid loop)
  setSpeedPID(left_speed, right_speed);

}

/* This function is not in use because we would like to control the timing that the loop is called */
void loop() {
  // do not use
}
