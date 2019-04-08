/* control.cpp */


// Libraries
#include <DueTimer.h>

// General
#include "control.h"
#include "../settings.h"
#include "../devices/encoders.h"
#include "../devices/motors.h"
#include "../util/conversions.h"


typedef struct {
    double  set_speed;          // The speed we are trying to achieve (in mm/sec)
    double  int_error;          // The summation of errors over time
    int     ticks_travelled;    // How far each wheel has travelled since last call to distanceTravelled()
} controller_state_t;

/* Stores the state of the controller for each motor, left first, then right */
volatile controller_state_t controllers[] = {
    {
        .set_speed = 0,
        .int_error = 0,
        .ticks_travelled = 0
    },
    {
        .set_speed = 0,
        .int_error = 0,
        .ticks_travelled = 0
    },
};

/* initialize control
 * starts the PID loop with a speed of 0 on each motor */
void initializeControl(void) {
    setSpeedPID(0.0, 0.0);

    Timer2.attachInterrupt(speedController).start(CONTROL_LOOP_TIME);
}

/* distance travelled
 * The control subsystem keeps track of how far each wheel
 * has travelled, this function is provided so that the
 * main loop can get the distance travelled */
void distanceTravelled(double* left_distance, double* right_distance) {
    *left_distance = ticksToMM(controllers[LEFT].ticks_travelled);
    *right_distance = ticksToMM(controllers[RIGHT].ticks_travelled);
    controllers[LEFT].ticks_travelled = 0;
    controllers[RIGHT].ticks_travelled = 0;
}

/* set speed PID
 * Sets the speed that the control algorithm uses */
void setSpeedPID(double left_speed, double right_speed){
    noInterrupts();
    controllers[LEFT].set_speed = left_speed;
    controllers[RIGHT].set_speed = right_speed;
    interrupts();
}

/* speed controller
 * This function is a ISR to run the PID loop for both controllers */
void speedController(void) {
    
    static unsigned long prev_time = micros();

    int ticks;
    double cur_speed;
    double error;
    double output; // Value between 0 and 1
    unsigned long cur_time = micros();

    for (int i = 0; i < 2; i++){
        // Read encoder ticks, and update ticks_travelled
        ticks = readEncoder(i);
        controllers[i].ticks_travelled += ticks;

        // Calculate Speed we are going in mm/sec
        cur_speed = ticksToMM(ticks) / ((double)(cur_time-prev_time) / 1000000);

        // calculate error
        error = cur_speed - controllers[i].set_speed;

        // add to integral error sum (bounded)
        if (controllers[i].int_error + error > -1 * INT_BOUND && controllers[i].int_error + error < INT_BOUND) {
            controllers[i].int_error += error;
        }

        output = (-1 * MOTOR_TAU_P * error) + (-1 * MOTOR_TAU_I * controllers[i].int_error);

        // Limit output between -1 and 1
        if (output > 1) output = 1;
        if (output < -1) output = -1;

        setMotorPWM(i, output);
    }
    prev_time = cur_time;
}
