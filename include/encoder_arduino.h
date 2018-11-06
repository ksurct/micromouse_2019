#ifndef _ENCODER_ARDUINO_H_
#define _ENCODER_ARDUINO_H_
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void encoderISR1();
void encoderISR0();
void encoderSetup(uint8_T pinA, uint8_T pinB, uint8_T id);
int readEncoder(uint8_T id);

#ifdef __cplusplus
}
#endif
#endif //_ENCODER_ARDUINO_H_