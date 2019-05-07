#ifndef ARDUINO
#include "localization.h"
#include "localization_test_data.h"
#include "../testing.h"
#include "../settings.h"
#include "../util/conversions.h"


#define IS_BETWEEN_ERROR(x,y,e) (((x) < (y) + (e)) && ((x) > (y) - (e)))
#define ACCEPTABLE_ERROR 0.001

void print_maze_state() {
    printf("\nMaze State:\n");
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            printf("|XXXXXXX| %.3f\t", robot_maze_state.cells[x][y].north->exists);
        }
        printf("|\n|");
        for (int i = 0; i < 79; i++)
            printf("-");
        printf("|\n");

        for (int x = 0; x < 5; x++) {
            printf("| %.3f\t|\t", robot_maze_state.cells[x][y].west->exists);
        }
        printf("|\n|");

        for (int i = 0; i < 79; i++)
            printf("-");
        printf("|\n");
    }
    printf("\n");
}

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

    // Test mazeMappingAndMeasureStep
    initializeLocalization();
    
    sensor_reading_t sensor_test_data[NUM_SENSORS];
    sensor_test_data[0] = (sensor_reading_t){ .state = GOOD, .distance = 41 };
    sensor_test_data[1] = (sensor_reading_t){ .state = GOOD, .distance = 45  };
    sensor_test_data[2] = (sensor_reading_t){ .state = TOO_FAR, .distance = 255 };
    sensor_test_data[3] = (sensor_reading_t){ .state = GOOD, .distance = 47 };
    sensor_test_data[4] = (sensor_reading_t){ .state = GOOD, .distance = 50 };

    robot_location.x_mu = 84.03;
    robot_location.y_mu = 84.0;
    robot_location.theta_mu = 0.0;

    //robot_maze_state.cells[0][0].south->exists = 1.0;
    //robot_maze_state.cells[0][1].south->exists = 1.0;
    
    // printf("Robot_location: (%.3f, %.3f, %.3f)\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);

    mazeMappingAndMeasureStep(sensor_test_data);

    // printf("Robot_location: (%.3f, %.3f, %.3f)\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);

    // printf("North: %f\n", robot_maze_state.cells[1][1].north->exists);
    // printf("East: %f\n", robot_maze_state.cells[1][1].east->exists);
    // printf("South: %f\n", robot_maze_state.cells[1][1].south->exists);
    // printf("West: %f\n", robot_maze_state.cells[1][1].west->exists);
    
    // print_maze_state();

    // TEST_FAIL("not all tests written yet!!!");

    // Test localizeMeasureStep
    // TEST_FAIL("not all tests written yet!!!");

} TEST_FUNC_END("localization_test")

#endif // ARDUINO
