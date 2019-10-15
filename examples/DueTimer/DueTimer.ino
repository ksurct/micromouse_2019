/* DueTimer.ino */

// Libraries
#include <DueTimer.h>

// Function Declarations
void main_loop(void);
void setup(void);

int start_;

/* Entry point to the code for the robot, all initialization
 * of subsystems should be done here */
void setup() {

  Serial.begin(9600);

  start_ = micros();

  // Start main loop
  Timer0.attachInterrupt(main_loop);
  Timer0.start(10000); // 10 milliseconds
}


void main_loop() {
    Serial.println(micros()-start_);
}

/* This function is not in use because we would like to control the timing that the loop is called */
void loop(){
  // do not use
}
