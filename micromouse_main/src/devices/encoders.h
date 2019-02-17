/* encoders.h */

#ifndef _ENCODERS_H_
#define _ENCODERS_H_


typedef struct
{
    unsigned char id;
    unsigned char pinA;
    unsigned char pinB;
} encoder_t;

// Sets up an encoder with the following arguments: id, pinA, pinB
void encoderSetup(unsigned char id, unsigned char pinA, unsigned char pinB);

// Reads from an encoder given an id
int readEncoder(unsigned char);

#endif //_ENCODERS_H_
