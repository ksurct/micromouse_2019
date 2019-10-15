/* sensors.cpp */


#include <Arduino.h>

#include "sensors.h"
#include "../settings.h"
#include "../types.h"


/* Sensor type to hold data about sensors */
typedef struct {
    unsigned char address;
    bool setup;
} sensor_t;


// Function declarations
void printSensorData(sensor_reading_t* sensor_data);
sensor_state_t parseError(unsigned char status);


// Sensor object to communicate with all the the sensors
Adafruit_VL6180X vl6180x = Adafruit_VL6180X();

sensor_t sensors[NUM_SENSORS] = {
    {
        .address = SENSOR_0_ADDR,
        .setup = false
    },
    {
        .address = SENSOR_1_ADDR,
        .setup = false
    },
    {
        .address = SENSOR_2_ADDR,
        .setup = false
    },
    {
        .address = SENSOR_3_ADDR,
        .setup = false
    },
    {
        .address = SENSOR_4_ADDR,
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

bool sensorSetup() {

    bool found = true;

    Wire.begin(); // Important, we need this for tcaselect to work!

    for (int i = 0; i < NUM_SENSORS; i++) {
        
        tcaselect(sensors[i].address);

        if (!vl6180x.begin()) {
            found = false;
        } else {
            sensors[i].setup = true;
        }

        #ifdef DEBUG_SENSORS
            Serial.print("DEBUG_SENSORS: Sensor ");
            Serial.print(i);
            if (sensors[i].setup) {
                Serial.println(" was found");
            } else {
                Serial.println(" was not found");
            }
        #endif
    }

    delay(50);

    return found;
}

void readSensors(sensor_reading_t* sensor_data) {

    for (int i = 0; i < NUM_SENSORS; i++) {

        if (sensors[i].setup) {
            tcaselect(sensors[i].address);
            sensor_data[i].distance = vl6180x.readRange();
            sensor_data[i].state = parseError(vl6180x.readRangeStatus());
        } else {
            sensor_data[i].distance = 255;
            sensor_data[i].state = ERROR;
        }
    }

}

void printSensorData(sensor_reading_t* sensor_data) {
    
    Serial.print("DEBUG_SENSORS: ");
    for (int i = 0; i < NUM_SENSORS; i++) {
        switch(sensor_data[i].state) {
            case GOOD:       // Successful reading
                Serial.print("GOOD");
                break;
            case TOO_FAR:    // Object to far to detect
                Serial.print("TOO_FAR");
                break;
            case TOO_CLOSE:  // Object to close to detect
                Serial.print("TOO_CLOSE");
                break;
            case WAITING:    // Sensor has no measurement yet, disregard distance
                Serial.print("WAITING");
                break;
            case ERROR:      // Error in reading, disregard distance
                Serial.print("ERROR");
                break;
            default:
                Serial.print("WHAT THE FREAK!!!");
                break;
        }
        Serial.print(", ");
        Serial.print(sensor_data[i].distance);
        Serial.print(", ");
    }
    Serial.println();
}

sensor_state_t parseError(unsigned char status) {
    
    if (status == VL6180X_ERROR_NONE) {
        return GOOD;
    }

    // Some error occurred
    if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
        return ERROR;
    }
    else if (status == VL6180X_ERROR_ECEFAIL) {
        return TOO_FAR;
    }
    else if (status == VL6180X_ERROR_NOCONVERGE) {
        return TOO_FAR;
    }
    else if (status == VL6180X_ERROR_RANGEIGNORE) {
        return TOO_FAR;
    }
    else if (status == VL6180X_ERROR_SNR) {
        return ERROR;
    }
    else if (status == VL6180X_ERROR_RAWUFLOW) {
        return TOO_CLOSE;
    }
    else if (status == VL6180X_ERROR_RAWOFLOW) {
        return TOO_FAR;
    }
    else if (status == VL6180X_ERROR_RANGEUFLOW) {
        return TOO_CLOSE;
    }
    else if (status == VL6180X_ERROR_RANGEOFLOW) {
        return TOO_FAR;
    }
    
    return ERROR;
}
