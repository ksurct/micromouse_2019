/* types.h
 *
 * Defines global structs to move data between subsystems */


#ifndef _TYPES_H_
#define _TYPES_H_

#include <Arduino.h>

typedef struct {
    uint8_t address;
    uint8_t interruptPin;
    uint8_t distance;
    bool needsUpdated;
} sensor_t;


// Holds a gaussian value
typedef struct {
    double mean;
    double sigma2;
} gaussian_t;


/* gaussian_location_t
 * Defines the location of the robot in mm from
 * the bottom left of the maze(starting point), theta
 * is defined as the angle in degrees counter-clockwise
 * from the x-axis */
typedef struct {
    gaussian_t* x;
    gaussian_t* y;
    gaussian_t* theta;
} gaussian_location_t;


#endif //_TYPES_H_
