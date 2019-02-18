/* conversions.cpp */

#include <Arduino.h>

#include "conversions.h"
#include "../settings.h"


/* Converts encoder ticks to a distance in mm */
double ticksToMM(int ticks) {
    return (double) (TWO_PI * WHEEL_RADIUS) * ((ticks) / TICKS_PER_REVOLUTION);
}
