/* sensors.h
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

#include <Arduino.h>

#include "Adafruit_VL6180X.h"
#include "../settings.h"
#include "../types.h"

#ifndef _SENSORS_H_
#define _SENSORS_H_

extern sensor_t sensors[];

/* Set up all the sensors to be used 
 *  Returns: true if successful */
bool sensorSetup();

/* Read from all the sensors
 *  Returns: true if successful */
bool readSensors(sensor_t* sensor_data);

#endif //_SENSORS_H_
