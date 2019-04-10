/* sensors.cpp */


#include <Arduino.h>

#include "sensors.h"
#include "../settings.h"
#include "../types.h"


/* Sensor type to hold data about sensors */
typedef struct {
    unsigned char address;
    unsigned char interruptPin;
    bool needsUpdated;
    bool setup;
} sensor_t;


// Function declarations
sensor_state_t parseError(unsigned char status);


// Sensor object to communicate with all the the sensors
Adafruit_VL6180X vl6180x = Adafruit_VL6180X();

volatile sensor_t sensors[NUM_SENSORS] = {
    {
        .address = SENSOR_0_ADDR,
        .interruptPin = SENSOR_0_PIN,
        .needsUpdated = false,
        .setup = false
    },
    {
        .address = SENSOR_1_ADDR,
        .interruptPin = SENSOR_1_PIN,
        .needsUpdated = false,
        .setup = false
    },
    {
        .address = SENSOR_2_ADDR,
        .interruptPin = SENSOR_2_PIN,
        .needsUpdated = false,
        .setup = false
    },
    {
        .address = SENSOR_3_ADDR,
        .interruptPin = SENSOR_3_PIN,
        .needsUpdated = false,
        .setup = false
    },
    {
        .address = SENSOR_4_ADDR,
        .interruptPin = SENSOR_4_PIN,
        .needsUpdated = false,
        .setup = false
    }
};

/* Tells the multiplexer which sensor we would like to select */
void tcaselect(unsigned char i) {
    if (i > 7) return;

    digitalWrite(I2C_RESET_PIN, LOW);
    delay(1);
    digitalWrite(I2C_RESET_PIN, HIGH);
    
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
    delay(1);
}

/* Sensor interrupts */
void sensorISR0() { sensors[0].needsUpdated = true; }
void sensorISR1() { sensors[1].needsUpdated = true; }
void sensorISR2() { sensors[2].needsUpdated = true; }
void sensorISR3() { sensors[3].needsUpdated = true; }
void sensorISR4() { sensors[4].needsUpdated = true; }

bool sensorSetup() {

    bool found = true;

    // Setup Reset pin
    pinMode(I2C_RESET_PIN, OUTPUT);
    digitalWrite(I2C_RESET_PIN, HIGH);
    digitalWrite(I2C_RESET_PIN, LOW);
    digitalWrite(I2C_RESET_PIN, HIGH);

    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);

    Wire.setClock(10000); // Slow down the clock from 100 kHz to 10 kHz

    Wire.begin(); // Important, we need this for tcaselect to work!

    Serial.println();
    for (int i = 0; i < NUM_SENSORS; i++){
        tcaselect(sensors[i].address);
        if (!vl6180x.begin()) {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.println(" was not found");
            found = false;
        } else {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.println(" was found");
            sensors[i].setup = true;
        }
    }

    delay(50);
    // attachInterrupt(digitalPinToInterrupt(sensors[0].interruptPin), sensorISR0, FALLING);
    // attachInterrupt(digitalPinToInterrupt(sensors[1].interruptPin), sensorISR1, FALLING);
    // attachInterrupt(digitalPinToInterrupt(sensors[2].interruptPin), sensorISR2, FALLING);
    // attachInterrupt(digitalPinToInterrupt(sensors[3].interruptPin), sensorISR3, FALLING);
    // attachInterrupt(digitalPinToInterrupt(sensors[4].interruptPin), sensorISR4, FALLING);

    return found;
}

void readSensors(sensor_reading_t* sensor_data) {

    Serial.println("readSensors called");
    for (int i = 0; i < NUM_SENSORS; i++) {
        tcaselect(sensors[i].address);
        digitalWrite(3, LOW);
        delay(1);
        digitalWrite(3, HIGH);
        Serial.print("Reading ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(vl6180x.readRange());
        Serial.print(" Status: ");
        Serial.println(parseError(vl6180x.readRangeStatus()));
        Serial.println("Hello Isaiah!");
        delay(50);
    }

    // for (int i = 0; i < NUM_SENSORS; i++) {
        
    //     Serial.println("Testing sensor");
    //     if (sensors[i].needsUpdated && sensors[i].setup) {
            
    //         tcaselect(sensors[i].address);
    //         Serial.println("Selected Sensor");
    //         sensor_data[i].distance = vl6180x.readRange();
    //         Serial.print("Reading: ");
    //         Serial.println(sensor_data[i].distance);
    //         sensor_data[i].state = parseError(vl6180x.readRangeStatus());
    //         sensors[i].needsUpdated = false;

    //     } else {
    //         sensor_data[i].distance = -1;
    //         sensor_data[i].state = waiting;
    //     }
    // }
}

sensor_state_t parseError(unsigned char status) {
    
    if (status == VL6180X_ERROR_NONE) {
        return good;
    }

    // Some error occurred
    if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
        return error;
    }
    else if (status == VL6180X_ERROR_ECEFAIL) {
        return too_far;
    }
    else if (status == VL6180X_ERROR_NOCONVERGE) {
        return too_far;
    }
    else if (status == VL6180X_ERROR_RANGEIGNORE) {
        return too_far;
    }
    else if (status == VL6180X_ERROR_SNR) {
        return error;
    }
    else if (status == VL6180X_ERROR_RAWUFLOW) {
        return too_close;
    }
    else if (status == VL6180X_ERROR_RAWOFLOW) {
        return too_far;
    }
    else if (status == VL6180X_ERROR_RANGEUFLOW) {
        return too_close;
    }
    else if (status == VL6180X_ERROR_RANGEOFLOW) {
        return too_far;
    }
    
    return error;
}
