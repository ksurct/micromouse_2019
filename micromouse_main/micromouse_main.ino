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
enum ButtonState {
  BUTTON_TRIGGER, // Only in this state once per button press
  BUTTON_WAIT,    // Waiting for button to go from high to low
  BUTTON_IDLE     // Waiting for button to go from low to high
};

void (*current_loop)(void); // Which loop should we be running

// Function Declarations
void movement_loop(void);
enum ButtonState button_state_machine(enum ButtonState state);
void robot_delay(unsigned long delay_time);

// loop states
void main_loop(void);
void idle_loop(void);


/* Entry point to the code for the robot, all initialization
   of subsystems should be done here */
void setup() {

  bool success;

  // Initialize Serial
  Serial.begin(115200);

  Serial.println("\n\nKSURCT Micromouse TEAM for 2018-2019\n");

  // Setup LEDs
  ledSetup();

  // Wait 2 seconds for humans to go away
  robot_delay(SETUP_TIME);

  // Setup pin for reset button
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

  // Initialize Control subsystem
  initializeControl();

  current_loop = &main_loop;

  Timer3.attachInterrupt(movement_loop).start(MOVEMENT_LOOP_TIME);
}


void loop() {

  static enum ButtonState button_state = BUTTON_IDLE;
  
  button_state = button_state_machine(button_state);

  if (current_loop == &main_loop){
    
    if (button_state == BUTTON_TRIGGER) {
      current_loop = &idle_loop;
      Timer3.stop();
    }
  } else if (current_loop == &idle_loop) {

    if (button_state == BUTTON_TRIGGER) {
      current_loop = &main_loop;
      Timer3.start(MOVEMENT_LOOP_TIME);
    }
  } else {
    // Something is going wrong, sound the alarms
    // Set LEDS high and go to idle loop
    current_loop = &idle_loop;
  }

  current_loop();
}

void main_loop(void) {

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

  // Update both maze state and robot location with sensor readings
  mazeMappingAndMeasureStep(sensor_data);

  #ifdef DEBUG_LOCALIZE_MAPPING
    printLocalizeMapping();
  #endif

  #ifdef DEBUG_LOCALIZE_MEASURE
    printLocalizeMeasure();
  #endif

}

void idle_loop(void) {

  static unsigned long idle_timer = millis();

  // This is part of debounceing the button while in the idle state
  while (millis() - idle_timer < IDLE_LOOP_TIME);
  
  idle_timer += IDLE_LOOP_TIME;

  #ifdef DEBUG_IDLE_TIMER
    Serial.print("DEBUG_IDLE_TIMER: ");
    Serial.println(idle_timer);
  #endif
}

void movement_loop(void) {
  // Initialize variables
  static double left_distance;
  static double right_distance;
  static double left_speed;
  static double right_speed;
  static gaussian_location_t next_location = {
      .x_mu = 84.0,
      .y_mu = 84.0
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

enum ButtonState button_state_machine(enum ButtonState state) {
  
  char button = digitalRead(RESET_PIN);

  switch (state) {
    case BUTTON_TRIGGER:
      state = BUTTON_WAIT;
      break;
    case BUTTON_WAIT:
      if (button == HIGH) {
        state = BUTTON_IDLE;
      }
      break;
    case BUTTON_IDLE:
      if (button == LOW) {
        state = BUTTON_TRIGGER;
      }
      break;
    default:
      // Something has gone wrong
      state = BUTTON_IDLE;
      break;
  }
  return state;
}

// Delay for time, time and flash leds
void robot_delay(unsigned long delay_time) {
  unsigned long end_time = millis() + delay_time;
  
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
