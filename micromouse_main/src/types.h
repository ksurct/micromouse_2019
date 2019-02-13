/* types.h
 *
 * Defines global structs to move data between subsystems */


#ifndef _TYPES_H_
#define _TYPES_H_

/* location_t
 * Defines the location of the robot in mm from
 * the bottom left of the maze(starting point), theta
 * is defined as the angle in degrees counter-clockwise
 * from the x-axis */
typedef struct {
    double x;
    double y;
    double theta;
} location_t;

#endif //_TYPES_H_
