/* Micromouse 2019 */


// Libraries
#include <DueTimer.h>

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
#include "src/movement/movement.h"
#include "src/control/control.h"

// Utilities
#include "src/util/conversions.h"


// Globals
unsigned long timer;
gaussian_location_t next_location = {.x_mu = 263.0, .y_mu = 264.0};
void (*current_loop)(void);

// Function Declarations
void sensor_loop(void);
void control_loop(void);
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

  // Wait 2 seconds for humans to go away
  robot_delay(SETUP_TIME);

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

  // Start control_loop()
  Timer2.attachInterrupt(control_loop).start(CONTROL_LOOP_TIME);

  // Initialize timer and starting loop
  timer = millis();

  current_loop = &sensor_loop;

}

void loop() {
  // Track if we go over the allocated time for a loop
  static unsigned long start_of_loop;

  
  if (millis() - timer > MAIN_LOOP_TIME) {
    start_of_loop = millis();

    current_loop();

    #ifdef DEBUG_TIMER
      Serial.print("DEBUG_TIMER: ");
    #endif

    // Reset timer
    while (millis() - timer > MAIN_LOOP_TIME) {
      timer += MAIN_LOOP_TIME;

      #ifdef DEBUG_TIMER
        Serial.print(timer);
        Serial.print(", ");
      #endif
    }
    #ifdef DEBUG_TIMER
      Serial.println();
    #endif
  }
}

void sensor_loop(void) {
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
    //printLocalizeMeasure();
  #endif

  // Determine next cell to go to (strategy step)
  //strategy(&robot_location, &robot_maze_state, &next_location);
}

void control_loop() {
  // Initialize variables
  static double left_distance;
  static double right_distance;
  static double left_speed;
  static double right_speed;

  // Heartbeat
  toggleLED(2);

  // Get distance travelled from control subsystem
  distanceTravelled(&left_distance, &right_distance);

  // Run distances through localization
  localizeMotionStep(left_distance, right_distance);

  #ifdef DEBUG_LOCALIZE_MOTION
    printLocalizeMotion();
  #endif

  // Determine what speed to set the motors to (speed profile + error correction, or turning profile + error correction)
  calculateSpeed(&robot_location, &next_location, &left_speed, &right_speed);

  #ifdef DEBUG_MOVEMENT
    //printSpeedData(left_speed, right_speed);
  #endif

  // Set speed using the motor controllers (pid loop)
  setSpeedPID(left_speed, right_speed);

  speedController();

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
