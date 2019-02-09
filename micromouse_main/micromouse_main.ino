/* Micromouse 2019 */

// Libraries
#include <DueTimer.h>

// Includes
#include "src/settings.h"
#include "src/devices/encoders.h"
#include "src/devices/sensors.h"
#include "src/devices/motors.h"

#include "src/util/conversions.h"


void main_loop() {

  // Initalize variables
  static sensor_t sensor_data[NUM_SENSORS];

  // Get sensor data
  if (!readSensors(sensor_data)){
    // throw error and log to serial
  }
  
  // Get encoder data
  int left_distance = ticksToMM(readEncoder(LEFT)); // Left encoder
  int right_distance = ticksToMM(readEncoder(RIGHT)); // Right encoder

  // Interpolate sensor and encoder data together using a kalman filter (measurement step)
  //location_t* current_position = localization(sensor_data, left_distance, right_distance);


  // Check if in a new cell
  // update map of the maze

  // if (at goal)
    // CheckStrategy(maze, location) to get new goal
  
  // determine what speed to set the motors to (speed profile)

  // Run predictions through the kalman filter (motion step)

  // Set speed using motor controller (pid loop)

}

void setup() {
  // Setup Sensors
  if (! sensorSetup()){
    // Throw error? Serial message maybe?
  }
  
  // Setup Encoders
  encoderSetup(LEFT, 1, 2); // id for left encoder, pinA, pinB
  encoderSetup(RIGHT, 3, 4); // id for right encoder, pinA, pinB
  
  Timer0.attachInterrupt(main_loop);
  Timer0.start(MAIN_LOOP_TIME);
}

void loop(){
  // do not use
}
