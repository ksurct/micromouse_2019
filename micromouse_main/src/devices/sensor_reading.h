/* sensor_reading.h */


#ifndef _SENSOR_READING_H_
#define _SENSOR_READING_H_


/* The reported state of a sensor */
enum sensor_state_t {
    good,       // Successful reading
    too_far,    // Object to far to detect
    too_close,  // Object to close to detect
    waiting,    // Sensor has no measurement yet, disregard distance
    error       // Error in reading, disregard distance
};


/* A single sensor reading */
typedef struct {
    sensor_state_t state;   // The reported state of the sensor
    unsigned char distance; // The reported distance of the sensor's measurement in mm
} sensor_reading_t;


#endif //_SENSOR_READING_H_
