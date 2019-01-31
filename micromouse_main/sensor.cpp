/* sensor.cpp */


#include "sensor.h"

#define TCAADDR 0x70

// Sensor object to communicate with all the the sensors
Adafruit_VL6180X vl6180x = Adafruit_VL6180X();

void tcaselect(uint8_t i) {
  if (i > 7) return;

  digitalWrite(I2C_RESET_PIN, LOW);
  delay(1);
  digitalWrite(I2C_RESET_PIN, HIGH);
  
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}


bool sensorSetup(uint8_t sensorCount, uint8_t* sensors) {
  
  // Important, we need this for tcaselect to work!
  Wire.begin();

  for (int i = 0; i < sensorCount; i++){
    tcaselect(sensors[i]);
    if (! vl6180x.begin()) {
      return false;
    }
  }
  return true;
}

bool readSensors(uint8_t sensorCount, uint8_t* sensors, int8_t* measurements) {
  return true;
}
