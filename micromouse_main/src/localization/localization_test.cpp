#ifndef ARDUINO
#include "localization.h"
#include "localization_test_data.h"
#include "../testing.h"
#include "../settings.h"


#define IS_BETWEEN_ERROR(x,y,e) (((x) < (y) + (e)) && ((x) > (y) - (e)))
#define ACCEPTABLE_ERROR 0.001


TEST_FUNC_BEGIN {
    
/* Test robot_location setup */

    initializeLocalization();
    TEST_PASS("initializeLocalization called");

    if (robot_location.x_mu != INIT_X_MU ) {
        TEST_FAIL("robot_location setup");
        goto after_robot_setup;
    }
    if (robot_location.x_sigma != INIT_X_SIGMA ) {
        TEST_FAIL("robot_location setup");
        goto after_robot_setup;
    }
    if (robot_location.xy_sigma != INIT_XY_SIGMA ) {
        TEST_FAIL("robot_location setup");
        goto after_robot_setup;
    }
    if (robot_location.y_mu != INIT_Y_MU ) {
        TEST_FAIL("robot_location setup");
        goto after_robot_setup;
    }
    if (robot_location.y_sigma != INIT_Y_SIGMA ) {
        TEST_FAIL("robot_location setup");
        goto after_robot_setup;
    }
    if (robot_location.theta_mu != INIT_THETA_MU ) {
        TEST_FAIL("robot_location setup");
        goto after_robot_setup;
    }
    if (robot_location.theta_sigma != INIT_THETA_SIGMA ) {
        TEST_FAIL("robot_location setup");
        goto after_robot_setup;
    }

    TEST_PASS("robot_location setup");
    after_robot_setup:
    ;

/* Test calculateMotion */

    // gaussian_location_t motion;
    // calculateMotion(&motion, 10, 15);

    // printf("motion:\n");
    // printf("x: %f, y: %f, theta: %f\n", motion.x_mu, motion.y_mu, motion.theta_mu);

    // addMotion(&motion);

    // printf("robot_location:\n");
    // printf("x: %f, y: %f, theta: %f\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);


/* Test localizeMotionStep */

    for (int i = 0; i < LOCALIZATION_TEST_DATA_ROWS; i++) {
        robot_location.x_mu = localization_test_data[i][2];
        robot_location.y_mu = localization_test_data[i][3];
        robot_location.theta_mu = localization_test_data[i][4];

        localizeMotionStep(localization_test_data[i][0], localization_test_data[i][1]);

        if ( !IS_BETWEEN_ERROR(robot_location.x_mu, localization_test_data[i][5], ACCEPTABLE_ERROR) ) {
            TEST_FAIL("localize motion step");
            goto after_localize_motion_step;
        }
        if ( !IS_BETWEEN_ERROR(robot_location.y_mu, localization_test_data[i][6], ACCEPTABLE_ERROR) ) {
            TEST_FAIL("localize motion step");
            goto after_localize_motion_step;
        }
        if ( !IS_BETWEEN_ERROR(robot_location.theta_mu, localization_test_data[i][7], ACCEPTABLE_ERROR) ) {
            
            if ( IS_BETWEEN_ERROR(robot_location.theta_mu, 0.0, ACCEPTABLE_ERROR) || IS_BETWEEN_ERROR(robot_location.theta_mu, TWO_PI, ACCEPTABLE_ERROR) ) {

                if ( IS_BETWEEN_ERROR(localization_test_data[i][7], 0.0, ACCEPTABLE_ERROR) || IS_BETWEEN_ERROR(localization_test_data[i][7], TWO_PI, ACCEPTABLE_ERROR) ) {
                    continue;
                }
            }

            printf("Failed:");
            for (int j = 0; j < LOCALIZATION_TEST_DATA_COL; j++) {
                printf("%f,\t", localization_test_data[i][j]);
            }
            printf("\n");
            
            printf("Calculated(%d): %f,\t%f,\t%f\n", i, robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);

            TEST_FAIL("localize motion step");
            goto after_localize_motion_step;
        }
    }

    TEST_PASS("localize motion step");
    after_localize_motion_step:
    ;

    // Test mazeMapping
    initializeLocalization();
    
    sensor_reading_t sensor_test_data[NUM_SENSORS];

    mazeMapping(sensor_test_data);
    
    TEST_FAIL("not all tests written yet!!!");

    // Test localizeMeasureStep
    // TEST_FAIL("not all tests written yet!!!");

} TEST_FUNC_END("localization_test")

#endif // ARDUINO
