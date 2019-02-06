/* encoders.h */

#ifndef _ENCODERS_H_
#define _ENCODERS_H_

typedef struct
{
    uint_t id;
    uint8_t pinA;
    uint8_t pinB;
} encoder_t;

extern encoder_t encoders[];

// Sets up an encoder with the following arguments: id, pinA, pinB
void encoderSetup(uint8_t id, uint8_t pinA, uint8_t pinB);

// Reads from an encoder given an id
int readEncoder(uint8_t);

#endif //_ENCODERS_H_
