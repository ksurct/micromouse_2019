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


#define TIME_STEP (double)(CONTROL_LOOP_TIME/1000000.0)  // in sec


bool goTo(gaussian_location_t* final_loc, int max_steps, bool debug) {
    
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

    if (debug) {
        printf("Step\t    DL\t\t    DR\t\t     X\t\t     Y\t\t  theta\n");
        
        printf("Start,");
        printf("\t%f,\t%f,", 0, 0);
        printf("\t%f,\t%f,\t%f;\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);
    }

    // While not at final location
    do {

        calculateSpeed(&robot_location, final_loc, &left_speed, &right_speed);
        left_q.push(left_speed); right_q.push(right_speed);

        localizeMotionStep(TIME_STEP * left_q.front(), TIME_STEP * right_q.front());

        if (debug) {
            printf("%d,", steps);
            printf("\t%f,\t%f,", left_q.front(), right_q.front());
            printf("\t%f,\t%f,\t%f;\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);
        }

        left_q.pop(); right_q.pop();

        ++steps;

        if (steps > max_steps) {
            break;
        }
    } while(true);
    // } while (!IS_BETWEEN_ERROR(robot_location.x_mu, final_loc->x_mu, TOLERANCE_MM) ||
    //          !IS_BETWEEN_ERROR(robot_location.y_mu, final_loc->y_mu, TOLERANCE_MM));

    // if not at goal
    if ( !IS_BETWEEN_ERROR(robot_location.x_mu, final_loc->x_mu, TOLERANCE_MM) ||
         !IS_BETWEEN_ERROR(robot_location.y_mu, final_loc->y_mu, TOLERANCE_MM) )
            return false;
    
    return true;
}


TEST_FUNC_BEGIN {

    // gaussian_location_t robot_location; // Use localization's robot_location instead
    gaussian_location_t final_loc;

    initializeLocalization();

    int max_steps;

/* Test standing still */

    // Maximum steps allowed
    max_steps = 100;

    // Initial location
    robot_location.x_mu = 100;
    robot_location.y_mu = 100;
    robot_location.theta_mu = directionToRAD[East];

    // Final location
    final_loc.x_mu = 100;
    final_loc.y_mu = 100;

    if (!goTo(&final_loc, max_steps, false))
        TEST_FAIL("Test standing still");
    else
        TEST_PASS("Test standing still");


/* Test going straight */

    // Maximum steps allowed
    max_steps = 200;

    // Initial location
    robot_location.x_mu = cellNumberToCoordinateDistance(0);
    robot_location.y_mu = cellNumberToCoordinateDistance(0);
    robot_location.theta_mu = directionToRAD[East];

    // Final location
    final_loc.x_mu = cellNumberToCoordinateDistance(1);
    final_loc.y_mu = cellNumberToCoordinateDistance(0);

    if (!goTo(&final_loc, max_steps, false))
        TEST_FAIL("Test going straight (perfect)");
    else
        TEST_PASS("Test going straight (perfect)");


/* Test going straight */

    // Maximum steps allowed
    max_steps = 200;

    // Initial location
    robot_location.x_mu = cellNumberToCoordinateDistance(0);
    robot_location.y_mu = cellNumberToCoordinateDistance(0) + (TOLERANCE_MM-1);
    robot_location.theta_mu = directionToRAD[East];

    // Final location
    final_loc.x_mu = cellNumberToCoordinateDistance(1);
    final_loc.y_mu = cellNumberToCoordinateDistance(0);

    if (!goTo(&final_loc, max_steps, false))
        TEST_FAIL("Test going straight (pid)");
    else
        TEST_PASS("Test going straight (pid)");


/* Test turning then going straight */

    // Maximum steps allowed
    max_steps = 1000;

    // Initial location
    robot_location.x_mu = cellNumberToCoordinateDistance(0);
    robot_location.y_mu = cellNumberToCoordinateDistance(0);
    robot_location.theta_mu = directionToRAD[South];

    // Final location
    final_loc.x_mu = cellNumberToCoordinateDistance(1);
    final_loc.y_mu = cellNumberToCoordinateDistance(0);

    if (!goTo(&final_loc, max_steps, true))
        TEST_FAIL("Test turning then going straight");
    else
        TEST_PASS("Test turning then going straight");


/* Test Other */

    // Still need to write tests
    TEST_FAIL("not all tests written yet!!!");

} TEST_FUNC_END("strategy_test")

#endif // ARDUINO
