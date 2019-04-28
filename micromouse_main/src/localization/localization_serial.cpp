
/* localization_serial.cpp */


#include <Arduino.h>

#include "localization.h"


// Prints out the robots current position for debugging
void printLocalizeMotion() {
    Serial.print("DEBUG_LOCALIZE_MOTION: ");
    Serial.print(robot_location.x_mu);
    Serial.print(", ");
    Serial.print(robot_location.y_mu);
    Serial.print(", ");
    Serial.print(robot_location.theta_mu);
    Serial.println();
}

void printLocalizeMapping() {
    //doTheThing();
}

void printLocalizeMeasure() {
    //doTheThing();    
}