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

unsigned long timer;

/* Entry point to the code for the robot, all initialization
   of subsystems should be done here */
void setup() {

  bool success;

  // Initialize Serial
  Serial.begin(115200);

  Serial.println("\n\nKSURCT Micromouse TEAM for 2018-2019\n");

  // Wait 2 seconds for human to go away
  delay(2000);

  // Setup Sensors
  success = sensorSetup();
  
  if (!success) {
    Serial.println("Error connecting to sensors!");
  } else {
    Serial.println("Sensors connected successfully");
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

  timer = millis();

//  Serial.println("Starting start loop");
//  Timer1.attachInterrupt(start_loop).start(MAIN_LOOP_TIME);
//  Serial.println("Loop Started");

  // // Start start loop
  //while (robot_location.theta_mu < 3*PI/2) {
    // Serial.println("Test");
    // Serial.println(robot_location.theta_mu);
  //}
  //Serial.println("Should have stopped");
  //Timer1.stop();
  
  // // Start main loop
  //Timer1.attachInterrupt(main_loop).start(MAIN_LOOP_TIME);
}

/* This function is not in use because we would like to control the timing that the loop is called */
void loop() {
  // Initialize variables
  static sensor_reading_t sensor_data[NUM_SENSORS];
  static double left_distance;
  static double right_distance;
  static double left_speed;
  static double right_speed;
  static gaussian_location_t next_location = {.x_mu = 264.0, .y_mu = 84.0};

//  Serial.println(millis());
//  Serial.println(timer);
  
  if (millis() - timer > MAIN_LOOP_TIME) {
    Serial.println(millis());
    // Get sensor data
    readSensors(sensor_data);
  
    // Get distance travelled from control subsystem
    distanceTravelled(&left_distance, &right_distance);
  
    // Run distances through localization
    localizeMotionStep(left_distance, right_distance);
  
    // Update maze with sensor readings
    mazeMappingAndMeasureStep(sensor_data);
  
    // Determine next cell to go to (strategy step)
    //strategy(&robot_location, &robot_maze_state, &next_location);
  
    // Determine what speed to set the motors to (speed profile + error correction, or turning profile + error correction)
    calculateSpeed(&robot_location, &next_location, &left_speed, &right_speed);
  
    // Set speed using the motor controllers (pid loop)
    setSpeedPID(left_speed, right_speed);

    // Reset timer
    Serial.println(millis());
    Serial.println(timer);
    Serial.println();
    timer += MAIN_LOOP_TIME;
  }
}
