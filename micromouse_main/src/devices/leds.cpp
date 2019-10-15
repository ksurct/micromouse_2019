/* leds.cpp */


#include <Arduino.h>

#include "leds.h"
#include "../settings.h"


/* Set up all the leds to be used */
void ledSetup() {
    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
    pinMode(LED_3_PIN, OUTPUT);
    setAllLEDSLow();
}

void setHighLED(char led_number) {
    switch (led_number) {
        case 1:
            digitalWrite(LED_1_PIN, HIGH);
            break;
        case 2:
            digitalWrite(LED_2_PIN, HIGH);
            break;
        case 3:
            digitalWrite(LED_3_PIN, HIGH);
            break;
        default:
            break;
    }
}

void setLowLED(char led_number) {
    switch (led_number) {
        case 1:
            digitalWrite(LED_1_PIN, LOW);
            break;
        case 2:
            digitalWrite(LED_2_PIN, LOW);
            break;
        case 3:
            digitalWrite(LED_3_PIN, LOW);
            break;
        default:
            break;
    }
}

void toggleLED(char led_number) {
    switch (led_number) {
        case 1:
            digitalWrite(LED_1_PIN, !digitalRead(LED_1_PIN));
            break;
        case 2:
            digitalWrite(LED_2_PIN, !digitalRead(LED_2_PIN));
            break;
        case 3:
            digitalWrite(LED_3_PIN, !digitalRead(LED_3_PIN));
            break;
        default:
            break;
    }
}

void flashLED(char led_number) {
    toggleLED(led_number);
    delay(1);
    toggleLED(led_number);
}

void setAllLEDSHigh() {
    digitalWrite(LED_1_PIN, HIGH);
    digitalWrite(LED_2_PIN, HIGH);
    digitalWrite(LED_3_PIN, HIGH);
}

void setAllLEDSLow() {
    digitalWrite(LED_1_PIN, LOW);
    digitalWrite(LED_2_PIN, LOW);
    digitalWrite(LED_3_PIN, LOW);
}
