/* leds.h
 * 
 * Control the LEDS on the robot  
 */

#ifndef _LEDS_H_
#define _LEDS_H_


/* Set up all the leds to be used */
void ledSetup();

void setHighLED(char led_number);

void setLowLED(char led_number);

void toggleLED(char led_number);

void flashLED(char led_number);

void setAllLEDSHigh();

void setAllLEDSLow();


#endif //_LEDS_H_
