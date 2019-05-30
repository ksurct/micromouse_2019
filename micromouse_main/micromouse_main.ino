/* Micromouse 2019 */


// General
#include "src/settings.h"
#include "src/types.h"

// Devices
#include "src/devices/leds.h"
#include "src/devices/sensors.h"
#include "src/devices/motors.h"
#include "src/devices/encoders.h"

// Subsystems
#include "src/localization/localization.h"
#include "src/localization/localization_serial.h"
#include "src/strategy/strategy.h"
#include "src/strategy/strategy_serial.h"
#include "src/movement/movement.h"
#include "src/control/control.h"

// Utilities
#include "src/util/conversions.h"

// Libraries
#include <DueTimer.h>


// Globals
unsigned long timer;
void (*current_loop)(void);

// Function Declarations
void go_to_idle_state(void);
void go_to_running_state(void);

void main_loop(void);
void movement_loop(void);
void idle_loop(void);

void robot_delay(unsigned long delay_time);


/* Entry point to the code for the robot, all initialization
   of subsystems should be done here */
void setup() {

  bool success;

  // Initialize Serial
  Serial.begin(115200);

  Serial.println("\n\nKSURCT Micromouse TEAM for 2018-2019\n");

  // Setup LEDs
  ledSetup();

  pinMode(RESET_PIN, INPUT_PULLUP);

  // Setup Sensors
  success = sensorSetup();
  
  if (!success) {
    setAllLEDSHigh();
  }

  #ifdef DEBUG_SENSORS
    if (!success)
      Serial.println("DEBUG_SENSORS: Error connecting to sensors!");
    else
      Serial.println("DEBUG_SENSORS: Sensors connected successfully");
  #endif

  // Setup Motors
  motorSetup();

  // Setup Encoders
  encoderSetup(LEFT, LEFT_ENCODER_PIN_A, LEFT_ENCODER_PIN_B);
  encoderSetup(RIGHT, RIGHT_ENCODER_PIN_A, RIGHT_ENCODER_PIN_B);

  // Initialize Localization subsystem
  initializeLocalization();

  // Initialize Strategy subsystem
  initializeStrategy();

  // Initialize timer and starting loop
  timer = millis();

  go_to_running_state();
}


void loop() {

  current_loop();

  if (done_flag) {
    go_to_idle_state();
  }

}

void go_to_idle_state(void) {
  done_flag = false;
  setAllLEDSHigh();
  current_loop = &idle_loop;
  Timer3.stop();
}

void go_to_running_state(void) {

  // Wait 2 seconds for humans to go away
  robot_delay(SETUP_TIME);
  
  // Initialize Control subsystem
  initializeControl();
  
  Timer3.attachInterrupt(movement_loop).start(MOVEMENT_LOOP_TIME);
  current_loop = &main_loop;
}

void main_loop(void) {
  // Initialize variables
  static sensor_reading_t sensor_data[NUM_SENSORS] = {
    (sensor_reading_t){ .state = ERROR,   .distance = 255 },
    (sensor_reading_t){ .state = ERROR,   .distance = 255 },
    (sensor_reading_t){ .state = ERROR,   .distance = 255 },
    (sensor_reading_t){ .state = ERROR,   .distance = 255 },
    (sensor_reading_t){ .state = ERROR,   .distance = 255 }
  };

  // Flash heartbeat
  flashLED(1);

  // Get sensor data
  readSensors(sensor_data);

  #ifdef DEBUG_SENSORS
    printSensorData(sensor_data);
  #endif

  // Update maze and robot location with sensor readings
  mazeMappingAndMeasureStep(sensor_data);

  #ifdef DEBUG_LOCALIZE_MAPPING
    printLocalizeMapping();
  #endif

  #ifdef DEBUG_LOCALIZE_MEASURE
    printLocalizeMeasure();
  #endif

}

void movement_loop(void) {
  // Initialize variables
  static double left_distance;
  static double right_distance;
  static double left_speed;
  static double right_speed;
  static gaussian_location_t next_location = {
      .x_mu = 264.0,
      .y_mu = 264.0
  };

  // Heartbeat
  toggleLED(2);

  // Get distance travelled from control subsystem
  distanceTravelled(&left_distance, &right_distance);

  // Run distances through localization
  localizeMotionStep(left_distance, right_distance);

  #ifdef DEBUG_LOCALIZE_MOTION
    printLocalizeMotion();
  #endif

  // Determine next cell to go to (strategy step)
  strategy(&robot_location, &robot_maze_state, &next_location);

  #ifdef DEBUG_STRATEGY
    printStrategy(&next_location);
  #endif

  // Determine what speed to set the motors to (speed profile + error correction, or turning profile + error correction)
  calculateSpeed(&robot_location, &next_location, &left_speed, &right_speed);

  #ifdef DEBUG_MOVEMENT
    //printSpeedData(left_speed, right_speed);
  #endif

  // Set speed using the motor controllers (pid loop)
  setSpeedPID(left_speed, right_speed);
}

void idle_loop(void) {

  // Check button
  if (digitalRead(RESET_PIN) == LOW) {
    go_to_running_state();
  }
}

// Delay for time, time and flash leds
void robot_delay(unsigned long delay_time) {
  unsigned long end_time = millis() + delay_time;
  
  setAllLEDSLow();

  while (millis() < end_time ) {
    toggleLED(1); delay(100); toggleLED(1);

    toggleLED(2); delay(100); toggleLED(2);

    toggleLED(3); delay(100); toggleLED(3);

    toggleLED(2); delay(100); toggleLED(2);
  }

  setAllLEDSHigh();
  delay(200);
  setAllLEDSLow();
}
