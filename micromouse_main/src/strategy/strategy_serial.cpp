/* strategy_serial.cpp */


#include <Arduino.h>

#include "../types.h"

#include "strategy.h"
#include "strategy_serial.h"


void printStrategy(gaussian_location_t * next_location) {
    Serial.print("DEBUG_STRATEGY: ");
    Serial.print("next_location: (");
    Serial.print(next_location->x_mu);
    Serial.print(", ");
    Serial.print(next_location->y_mu);
    Serial.print(", ");
    Serial.print(next_location->theta_mu);
    Serial.println(")");
}
