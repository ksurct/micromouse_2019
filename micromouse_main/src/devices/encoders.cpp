  /* encoders.cpp */


#include <Arduino.h>

#include "encoders.h"


#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

typedef struct {
  volatile uint32_t * pinA_register;
  volatile uint32_t * pinB_register;
  uint32_t pinA_bitmask;
  uint32_t pinB_bitmask;
  unsigned char state;
  int position;
} Encoder_state_t;

volatile Encoder_state_t encoders[2];

void encoderISR1() {
    static unsigned char id = 1;
    unsigned char s = encoders[id].state & 3;
    if (DIRECT_PIN_READ(encoders[id].pinA_register, encoders[id].pinA_bitmask)) s |= 4;
    if (DIRECT_PIN_READ(encoders[id].pinB_register, encoders[id].pinB_bitmask)) s |= 8;
    switch (s) {
        case 0: case 5: case 10: case 15:
            break;
        case 1: case 7: case 8: case 14:
            encoders[id].position++; break;
        case 2: case 4: case 11: case 13:
            encoders[id].position--; break;
        case 3: case 12:
            encoders[id].position += 2; break;
        default:
            encoders[id].position -= 2; break;
    }
    encoders[id].state = (s >> 2);
}

void encoderISR0() {
    static unsigned char id = 0;
    unsigned char s = encoders[id].state & 3;
    if (DIRECT_PIN_READ(encoders[id].pinA_register, encoders[id].pinA_bitmask)) s |= 4;
    if (DIRECT_PIN_READ(encoders[id].pinB_register, encoders[id].pinB_bitmask)) s |= 8;
    switch (s) {
        case 0: case 5: case 10: case 15:
            break;
        case 1: case 7: case 8: case 14:
            encoders[id].position++; break;
        case 2: case 4: case 11: case 13:
            encoders[id].position--; break;
        case 3: case 12:
            encoders[id].position += 2; break;
        default:
            encoders[id].position -= 2; break;
    }
    encoders[id].state = (s >> 2);
}

// Create an Encoder object and save it to the pointer
void encoderSetup(unsigned char id, unsigned char pinA, unsigned char pinB)
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);

    encoders[id].pinA_register = portInputRegister(digitalPinToPort(pinA));
    encoders[id].pinB_register = portInputRegister(digitalPinToPort(pinB));
    encoders[id].pinA_bitmask = digitalPinToBitMask(pinA);
    encoders[id].pinB_bitmask = digitalPinToBitMask(pinB);
    encoders[id].state = 0;
    encoders[id].position = 0;

    if (id == 1) {
        attachInterrupt(digitalPinToInterrupt(pinA), encoderISR1, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pinB), encoderISR1, CHANGE);
    } else if (id == 0) {
        attachInterrupt(digitalPinToInterrupt(pinA), encoderISR0, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pinB), encoderISR0, CHANGE);
    }
}

// Read the position relative to the old position
// and reset the position to zero
int readEncoder(unsigned char id) 
{ 
    int currentPosition = encoders[id].position;
    encoders[id].position = 0;
    return currentPosition;
}
