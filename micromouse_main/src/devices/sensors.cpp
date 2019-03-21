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
} sensor_t;


// Function declarations
sensor_state_t parseError(unsigned char status);


// Sensor object to communicate with all the the sensors
Adafruit_VL6180X vl6180x = Adafruit_VL6180X();

sensor_t sensors[NUM_SENSORS] = {
    {
        .address = SENSOR_0_ADDR,
        .interruptPin = SENSOR_0_PIN,
        .needsUpdated = false
    },
    {
        .address = SENSOR_1_ADDR,
        .interruptPin = SENSOR_1_PIN,
        .needsUpdated = false
    },
    {
        .address = SENSOR_2_ADDR,
        .interruptPin = SENSOR_2_PIN,
        .needsUpdated = false
    },
    {
        .address = SENSOR_3_ADDR,
        .interruptPin = SENSOR_3_PIN,
        .needsUpdated = false
    },
    {
        .address = SENSOR_4_ADDR,
        .interruptPin = SENSOR_4_PIN,
        .needsUpdated = false
    }
};

/* Tells the multiplexer which sensor we would like to select */
void tcaselect(unsigned char i) {
    if (i > 7) return;

    { // Is this section necessary?
        digitalWrite(I2C_RESET_PIN, LOW);
        delay(1);
        digitalWrite(I2C_RESET_PIN, HIGH);
    }
    
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

/* Sensor interrupts */
void sensorISR0() { sensors[0].needsUpdated = true; }
void sensorISR1() { sensors[1].needsUpdated = true; }
void sensorISR2() { sensors[2].needsUpdated = true; }
void sensorISR3() { sensors[3].needsUpdated = true; }
void sensorISR4() { sensors[4].needsUpdated = true; }

bool sensorSetup() {

    Wire.begin(); // Important, we need this for tcaselect to work!

    bool found = true;

    for (int i = 0; i < NUM_SENSORS; i++){
        tcaselect(sensors[i].address);
        if (! vl6180x.begin()) {
            found = false;
        }
    }

    attachInterrupt(digitalPinToInterrupt(sensors[0].address), sensorISR0, RISING);
    attachInterrupt(digitalPinToInterrupt(sensors[1].address), sensorISR1, RISING);
    attachInterrupt(digitalPinToInterrupt(sensors[2].address), sensorISR2, RISING);
    attachInterrupt(digitalPinToInterrupt(sensors[3].address), sensorISR3, RISING);
    attachInterrupt(digitalPinToInterrupt(sensors[4].address), sensorISR4, RISING);

    return found;
}

void readSensors(sensor_reading_t* sensor_data) {

    for (int i = 0; i < NUM_SENSORS; i++) {
        
        if (sensors[i].needsUpdated) {
            
            tcaselect(sensors[i].address);
            sensor_data[i].distance = vl6180x.readRange();
            sensor_data[i].state = parseError(vl6180x.readRangeStatus());
            sensors[i].needsUpdated = false;

        } else {
            sensor_data[i].distance = 0;
            sensor_data[i].state = waiting;
        }
    }
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
