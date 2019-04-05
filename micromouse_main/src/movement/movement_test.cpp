#ifndef ARDUINO
#include <math.h>
#include <time.h>   
#include "movement.h"
#include "../testing.h"
#include "../settings.h"
#include "../types.h"
#include "../util/conversions.h"
#include "../localization/localization.h"


#define TIME_STEP MAIN_LOOP_TIME

#define IS_BETWEEN_ERROR(x,y,e) (((x) < (y) + (e)) && ((x) > (y) - (e)))

// From localization, but modified
void robotModel(gaussian_location_t* current_loc, double left_distance, double right_distance) {

    gaussian_location_t motion;
    calculateMotion(&motion, left_distance, right_distance);

    current_loc->theta_mu = current_loc->theta_mu + motion.theta_mu;

    // Limit current_loc->theta_mu to be between 0 and 2 pi
    while (current_loc->theta_mu < 0) { current_loc->theta_mu = current_loc->theta_mu + TWO_PI; }
    while (current_loc->theta_mu > TWO_PI) { current_loc->theta_mu = current_loc->theta_mu - TWO_PI; }

    current_loc->x_mu = current_loc->x_mu + motion.x_mu * (cos(current_loc->theta_mu) - sin(current_loc->theta_mu));
    current_loc->y_mu = current_loc->y_mu + motion.y_mu * (sin(current_loc->theta_mu) + cos(current_loc->theta_mu));
}

bool goTo(gaussian_location_t* current_loc, gaussian_location_t* final_loc, int max_steps) {
    
    double left_speed;
    double right_speed;
    int steps = 0;

    // While not at final location
    while (!IS_BETWEEN_ERROR(current_loc->x_mu, final_loc->x_mu, 5) ||
           !IS_BETWEEN_ERROR(current_loc->y_mu, final_loc->y_mu, 5) ||
           !IS_BETWEEN_ERROR(current_loc->theta_mu, final_loc->theta_mu, 0.5)) {

        calculateSpeed(current_loc, final_loc, &left_speed, &right_speed);

        robotModel(current_loc, TIME_STEP * left_speed, TIME_STEP * right_speed);

        if (++steps > max_steps) {
            return false;
        }
    }
    return true;
}


TEST_FUNC_BEGIN {

    gaussian_location_t current_loc;
    gaussian_location_t final_loc;

    int max_steps;

/* Test standing still */

    max_steps = 1000;

    // Setup initial location and final location
    #define DUMMY 1
    current_loc.x_mu = DUMMY;
    current_loc.y_mu = DUMMY;
    current_loc.theta_mu = DUMMY;

    final_loc.x_mu = DUMMY;
    final_loc.y_mu = DUMMY;
    final_loc.theta_mu = DUMMY;

    if (!goTo(&current_loc, &final_loc, max_steps))
        TEST_FAIL("standing still");
    else
        TEST_PASS("standing still");


/* Test turning left */
/* Test turning right */
/* Test turning 180 degrees */
/* Test going straight */
/* Test going straight then turning left */
/* Test going straight then turning right */


    
    // Still need to write tests
    TEST_FAIL("not all tests written yet!!!");

} TEST_FUNC_END("strategy_test")

#endif // ARDUINO
