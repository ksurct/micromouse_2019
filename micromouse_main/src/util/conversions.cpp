/* conversions.cpp */


#include "conversions.h"
#include "../settings.h"


/* Converts encoder ticks to a distance in mm */
double ticksToMM(int ticks) {
    return (double) (TWO_PI * WHEEL_RADIUS) * ((ticks) / TICKS_PER_REVOLUTION);
}

/* Convert a coordinate distance measured in mm to a cell number that it is in */
int coordinateDistanceToCellNumber(double coordinateDistance) {
    return (int)(coordinateDistance / (WALL_THICKNESS + CELL_LENGTH));
}

/* Convert a cell number to a coordinate distance measured in mm that is the center of the cell */
double cellNumberToCoordinateDistance(int cellNumber) {
    return (double)((cellNumber * (WALL_THICKNESS + CELL_LENGTH)) + (CELL_LENGTH / 2));
}
