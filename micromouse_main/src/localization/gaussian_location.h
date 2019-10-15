/* gaussian_location.h */


#ifndef _GAUSSIAN_LOCATION_H_
#define _GAUSSIAN_LOCATION_H_


/* Set the location type to double */
typedef double location_t;

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
    location_t x_mu;           /* Mean x location */
    location_t y_mu;           /* Mean y location */
    location_t theta_mu;       /* Mean theta location */
    location_t x_sigma;       /* Covariance in x */
    location_t xy_sigma;      /* Covariance in x and y */
    location_t y_sigma;       /* Covariance in y */
    location_t theta_sigma;   /* Covariance in theta */
} gaussian_location_t;

#endif //_GAUSSIAN_LOCATION_H_
