/* Micromouse 2019 */

// Libraries
#include <DueTimer.h>

// General
#include "src/settings.h"
#include "src/types.h"

// Devices
#include "src/devices/encoders.h"
#include "src/devices/sensors.h"
#include "src/devices/motors.h"

// Subsystems
#include "src/localization/localization.h"
#include "src/localization/probabilistic_maze.h"

#include "src/strategy/strategy.h"

#include "src/control/control.h"


// Utilities
#include "src/util/conversions.h"

// Function Declarations
void setup(void);
void main_loop(void);


/* Entry point to the code for the robot, all initialization
 * of subsystems should be done here */
void setup() {

  // Initialize Serial
  Serial.begin(115200);

  // Setup Sensors
  // if (! sensorSetup()){
  //   // Throw error? Serial message maybe?
  // }

  Serial.println("Before motorSetup()");

  // Setup Motors
  motorSetup();

  // Setup Encoders
  encoderSetup(LEFT, LEFT_ENCODER_PIN_A, LEFT_ENCODER_PIN_B);
  encoderSetup(RIGHT, RIGHT_ENCODER_PIN_A, RIGHT_ENCODER_PIN_B);

  // Initialize Localization subsystem
  //initializeLocalization();

  // Initialize Strategy subsystem
  //initializeStrategy();

  Serial.println("Starting Control");

  // Initialize Control subsystem
  initializeControl();

  Serial.println("Control Started");

  Serial.println("Staring Main");  

  // // Start main loop
  Timer0.attachInterrupt(main_loop);
  Timer0.start(MAIN_LOOP_TIME);

  Serial.println("Main Started");
}


void main_loop() {
  static int MAX_SPEED = 100;
  static int MIN_SPEED = 50;

  // Initialize variables
  static sensor_t sensor_data[NUM_SENSORS];
  static double left_distance;
  static double right_distance;
  static double left_speed = MAX_SPEED;
  static double right_speed = MIN_SPEED;
  static gaussian_location_t next_location;

  // Get sensor data
  // if (!readSensors(sensor_data)){
  //   // throw error and log to serial
  // }

  // Get distance travelled from control subsystem
  distanceTravelled(&left_distance, &right_distance);

  // Interpolate sensor and encoder data together using a kalman filter (measurement step)
  //localizeMeasureStep(sensor_data, left_distance, right_distance);

  // Update maze with sensor readings
  //mazeMapping(sensor_data);

  // Determine next cell to go to (strategy step)
  //strategy(&robot_location, &robot_maze_state, &next_location);
  
  // Determine what speed to set the motors to (speed profile + error correction, or turning profile + error correction)
  //calculateSpeed(&robot_location, &next_location, &left_speed, &right_speed);

  // Run predictions through the kalman filter (motion step)
  //localizeMotionStep(left_speed * MAIN_LOOP_TIME, right_speed * MAIN_LOOP_TIME);

  

  // Set speed using the motor controllers (pid loop)
  setSpeedPID(MAX_SPEED, 0);

//  Serial.print("Distance: ");
//  Serial.print(left_distance);
//  Serial.print(" Speed: ");
//  Serial.println(left_speed);

}

/* This function is not in use because we would like to control the timing that the loop is called */
void loop(){
  // do not use
}
