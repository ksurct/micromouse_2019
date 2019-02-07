/* Micromouse 2019 */

// Libraries
#include <DueTimer.h>

// Includes
#include "settings.h"
#include "devices/encoders.h"
#include "devices/sensors.h"
#include "devices/motors.h"

void main_loop(){
  // Get sensor data
  readSensors();
  
  // Get encoder data
  readEncoder(0); // Left encoder
  readEncoder(1); // Right encoder

  // Interpolate sensor and encoder data together using a kalman filter (measurement step)

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
