/* sensor.h
 *  
 *  This file gives access to setting up Lidar Sensors for communication 
 *  over i2c. When working with these functions, the order of the sensor 
 *  numbers and measurements corresponds with the sensors in order from 
 *  back left, to back right in a clockwise rotation.
 *  
 *           sensor2
 *         |---------|
 *         |         |
 * sensor1 |         | sensor3
 *        /           \
 *       |             |
 *        \           /
 *         |         |
 * sensor0 |         | sensor4
 *         |---------|
 *         
 */

#include "Adafruit_VL6180X.h"
#include "settings.h"

#ifndef _SENSOR_H_
#define _SENSOR_H_

/* Set up all the sensors to be used 
 *  sensorCount: number of sensors
 *  sensors: multiplexer address in the order described above
 *  Returns: true if successful */
bool sensorSetup(uint8_t sensorCount, uint8_t* sensors);

/* Read from all the sensors
 *  sensorCount: number of sensors
 *  sensors: multiplexer address in the order described above
 *  measurements: the measurements taken by the sensors in the order described above
 *    - Note: measurement will be -1 if the sensor doesn't see anything or has an error
 *  Returns: true if successful */
bool readSensors(uint8_t sensorCount, uint8_t* sensors, int8_t* measurements);

#endif //_SENSOR_H_
