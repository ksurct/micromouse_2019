
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

// Prints out the current maze state
void printLocalizeMapping() {

    Serial.print("DEBUG_LOCALIZE_MAPPING: \r\n");
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            Serial.print("|XXXXXXX| ");
            Serial.print(robot_maze_state.cells[x][y].north->exists);
            Serial.print("\t");
        }
        Serial.print("|\r\n|-------------------------------------------------------------------------------|\r\n");

        for (int x = 0; x < 5; x++) {
            Serial.print("| ");
            Serial.print(robot_maze_state.cells[x][y].west->exists);
            Serial.print("\t|\t");
        }
        Serial.print("|\r\n|-------------------------------------------------------------------------------|\r\n");
    }
}

void printLocalizeMeasure() {
    //doTheThing();    
}