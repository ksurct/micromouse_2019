/* conversions.h
 * 
 * This file is responsible for defining conversion functions
 */


#ifndef _CONVERSIONS_H_
#define _CONVERSIONS_H_


/* Converts encoder ticks to a distance in mm */
double ticksToMM(int ticks);

/* Convert a coordinate distance measured in mm to a cell number that it is in */
int coordinateDistanceToCellNumber(double coordinateDistance);

/* Convert a cell number to a coordinate distance measured in mm that is the center of the cell */
double cellNumberToCoordinateDistance(int cellNumber);


#endif //_CONVERSIONS_H_
