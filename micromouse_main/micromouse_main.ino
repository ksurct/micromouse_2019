/* Micromouse 2019 */

// Libraries
#include <DueTimer.h>

// General
#include "src/settings.h"
#include "src/types.h"

// Devices
#include "src/devices/sensors.h"


// Function Declarations
void setup(void);
void main_loop(void);


/* Entry point to the code for the robot, all initialization
 * of subsystems should be done here */
void setup() {

  // Initialize Serial
  Serial.begin(115200);

  delay(500);

  Serial.println("\n\nRESET ARDUINO\n");

  // Setup Sensors 
  if (!sensorSetup()){
    Serial.println("Error connecting to sensors!");
  }

  // Start main loop
  //Timer1.attachInterrupt(main_loop);
  //Timer1.start(MAIN_LOOP_TIME);

}


void main_loop() {

  
}

/* This function is not in use because we would like to control the timing that the loop is called */
void loop(){
  // do not use
  static sensor_reading_t sensor_data[NUM_SENSORS];
  readSensors(sensor_data);
  for(int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": \t");
    Serial.print(sensor_data[i].distance);
    Serial.print(", \t");
    switch(sensor_data[i].state) {
      case good:       // Successful reading
        Serial.print("good");
        break;
      case too_far:    // Object to far to detect
        Serial.print("too_far");
        break;
      case too_close:  // Object to close to detect
        Serial.print("too_close");
        break;
      case waiting:    // Sensor has no measurement yet, disregard distance
        Serial.print("waiting");
        break;
      case error:      // Error in reading, disregard distance
        Serial.print("error");
        break;
      default:
        Serial.print("WHAT THE FREAK!!!");
        break;
    }
    Serial.println();
  }
  delay(500);
}
