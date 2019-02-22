/* types.h
 *
 * Defines global structs to move data between subsystems */


#ifndef _TYPES_H_
#define _TYPES_H_


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


/* Holds a gaussian value */
typedef struct {
    double mean;
    double sigma2;
} gaussian_t;


/* gaussian_location_t
 * Defines the location of the robot in mm from
 * the top left of the maze(starting point), theta
 * is defined as the angle in degrees counter-clockwise
 * from the x axis
 * 
 * (0,0) starts in the top left corner of the maze inside of the maze
 * +---------> +x axis
 * |
 * |
 * |
 * V
 * +y axis
 * 
 * */
typedef struct {
    gaussian_t x;
    gaussian_t y;
    gaussian_t theta;
} gaussian_location_t;


#endif //_TYPES_H_
