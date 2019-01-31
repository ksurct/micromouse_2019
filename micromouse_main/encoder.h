/* encoder.h */

#ifndef _ENCODER_H_
#define _ENCODER_H_

// Sets up an encoder with the following arguments: pinA, pinB, id
void encoderSetup(uint8_t, uint8_t, uint8_t);

// Reads from an encoder given an id
int readEncoder(uint8_t);

#endif //_ENCODER_H_
