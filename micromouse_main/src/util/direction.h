/* direction.h */


#ifndef _DIRECTION_H_
#define _DIRECTION_H_


enum Direction {
    North,  // North(0, -1) theta = 3*PI/2
    East,   // East(1, 0)   theta = 0
    South,  // South(0, 1)  theta = PI/2
    West,   // West(-1, 0)  theta = PI
};

extern double directionToRAD[4];

extern double directionToXY[4][2];


#endif //_DIRECTION_H_
