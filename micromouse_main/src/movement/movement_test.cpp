#ifndef ARDUINO
#include <math.h>
#include <time.h>
#include <queue>  
#include "movement.h"
#include "../testing.h"
#include "../settings.h"
#include "../types.h"
#include "../util/conversions.h"
#include "../localization/localization.h"

using namespace std;


#define TIME_STEP 1 // (double)(CONTROL_LOOP_TIME/1000000.0)  // in sec

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
    
    int lag = 0;
    queue<double> left_q;
    queue<double> right_q;
    for (int i = 0; i < lag; i++) {
        left_q.push(0.0);
        right_q.push(0.0);
    }

    printf("Step\t     X\t\t     Y\t\t  theta\n");

    printf("%d\t%f\t%f\t%f\n", steps, current_loc->x_mu, current_loc->y_mu, current_loc->theta_mu);

    // While not at final location
    do {

        calculateSpeed(current_loc, final_loc, &left_speed, &right_speed);
        left_q.push(left_speed); right_q.push(right_speed);

        printf("Speeds: %f %f --- ", left_q.front(), right_q.front());

        robotModel(current_loc, TIME_STEP * left_q.front(), TIME_STEP * right_q.front());
        left_q.pop(); right_q.pop();

        ++steps;

        printf("%d\t%f\t%f\t%f\n", steps, current_loc->x_mu, current_loc->y_mu, current_loc->theta_mu);

        if (steps > max_steps) {
            return false;
        }

    } while (!IS_BETWEEN_ERROR(current_loc->x_mu, final_loc->x_mu, TOLERANCE_MM) ||
             !IS_BETWEEN_ERROR(current_loc->y_mu, final_loc->y_mu, TOLERANCE_MM));

    return true;
}


TEST_FUNC_BEGIN {

    gaussian_location_t current_loc;
    gaussian_location_t final_loc;

    int max_steps;

/* Test standing still */

    // Maximum steps allowed
    max_steps = 1;

    // Initial location
    current_loc.x_mu = 100;
    current_loc.y_mu = 100;
    current_loc.theta_mu = directionToRAD[East];

    // Final location
    final_loc.x_mu = 100;
    final_loc.y_mu = 100;

    if (!goTo(&current_loc, &final_loc, max_steps))
        TEST_FAIL("Test standing still");
    else
        TEST_PASS("Test standing still");


/* Test going straight */

    // Maximum steps allowed
    max_steps = 500;

    // Initial location
    current_loc.x_mu = cellNumberToCoordinateDistance(0);
    current_loc.y_mu = cellNumberToCoordinateDistance(0);
    current_loc.theta_mu = directionToRAD[East];

    // Final location
    final_loc.x_mu = cellNumberToCoordinateDistance(1);
    final_loc.y_mu = cellNumberToCoordinateDistance(0);

    if (!goTo(&current_loc, &final_loc, max_steps))
        TEST_FAIL("Test going straight (perfect)");
    else
        TEST_PASS("Test going straight (perfect)");


/* Test going straight */

    // Maximum steps allowed
    max_steps = 10;

    // Initial location
    current_loc.x_mu = cellNumberToCoordinateDistance(0);
    current_loc.y_mu = cellNumberToCoordinateDistance(0) + (TOLERANCE_MM-1);
    current_loc.theta_mu = directionToRAD[East];

    // Final location
    final_loc.x_mu = cellNumberToCoordinateDistance(1);
    final_loc.y_mu = cellNumberToCoordinateDistance(0);

    if (!goTo(&current_loc, &final_loc, max_steps))
        TEST_FAIL("Test going straight (pid)");
    else
        TEST_PASS("Test going straight (pid)");
    
    printf("Model Test\n");
    gaussian_location_t robot;
    robot.x_mu = 0;
    robot.y_mu = 0;
    robot.theta_mu = 0;
    robotModel(&robot, 10, 11);
    printf("Robot: %f %f %f\n", robot.x_mu, robot.y_mu, robot.theta_mu);


/* Test turning */

    // // Maximum steps allowed
    // max_steps = 1000;

    // // Initial location
    // current_loc.x_mu = cellNumberToCoordinateDistance(0);
    // current_loc.y_mu = cellNumberToCoordinateDistance(0);
    // current_loc.theta_mu = directionToRAD[South];

    // // Final location
    // final_loc.x_mu = cellNumberToCoordinateDistance(1);
    // final_loc.y_mu = cellNumberToCoordinateDistance(0);

    // if (!goTo(&current_loc, &final_loc, max_steps))
    //     TEST_FAIL("Test turning");
    // else
    //     TEST_PASS("Test turning");


/* Test Other */

    // Still need to write tests
    TEST_FAIL("not all tests written yet!!!");

} TEST_FUNC_END("strategy_test")

#endif // ARDUINO
