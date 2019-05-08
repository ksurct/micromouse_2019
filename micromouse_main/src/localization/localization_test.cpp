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

    double measurements[][NUM_SENSORS] = {
        {40, 56, 40, 41, 56},
        {41, 56, 38, 41, 58},
        {40, 55, 41, 43, 58},
        {39, 54, 39, 41, 60},
        {40, 56, 40, 41, 57},
        {40, 55, 40, 41, 57},
        {42, 56, 43, 41, 59},
        {40, 53, 39, 45, 59},
        {41, 54, 40, 43, 58},
        {41, 53, 38, 41, 57},
        {39, 54, 39, 42, 60},
        {39, 56, 39, 42, 59},
        {41, 55, 41, 42, 58},
        {39, 54, 40, 41, 57},
        {41, 56, 41, 44, 59},
        {41, 55, 40, 42, 57},
        {41, 56, 40, 42, 58},
        {40, 54, 39, 42, 55},
        {41, 56, 39, 40, 57},
        {42, 56, 39, 40, 57},
        {41, 55, 41, 42, 59},
        {41, 56, 37, 43, 57},
        {40, 55, 38, 40, 58},
        {40, 54, 39, 42, 59},
        {41, 53, 41, 43, 60},
        {40, 53, 40, 43, 57},
        {39, 54, 41, 43, 57},
        {39, 56, 40, 41, 58},
        {42, 55, 40, 42, 58},
        {40, 55, 39, 42, 60},
        {39, 53, 39, 42, 57},
        {40, 55, 38, 42, 57},
        {39, 56, 42, 40, 56},
        {39, 52, 41, 41, 56},
        {41, 54, 39, 46, 57},
        {41, 55, 39, 42, 57},
        {40, 56, 39, 41, 57},
        {43, 54, 38, 41, 59},
        {39, 54, 41, 43, 58},
        {40, 54, 39, 41, 58},
        {40, 56, 39, 44, 57},
        {39, 53, 40, 41, 58},
        {42, 55, 40, 40, 60},
        {40, 55, 37, 41, 58},
        {40, 54, 39, 42, 58},
        {40, 55, 39, 42, 57},
        {41, 54, 39, 42, 57},
        {41, 57, 39, 42, 59},
        {41, 55, 39, 46, 59},
        {39, 53, 40, 42, 56},
        {40, 54, 41, 41, 58},
        {39, 55, 39, 44, 57},
        {41, 54, 39, 43, 58},
        {39, 55, 40, 43, 58},
        {42, 55, 42, 42, 59},
        {40, 54, 39, 41, 59},
        {40, 54, 39, 42, 59},
        {41, 54, 41, 42, 58},
        {43, 56, 39, 41, 57},
        {41, 54, 38, 42, 60},
        {40, 55, 39, 44, 57},
        {39, 53, 41, 42, 58},
        {41, 54, 40, 42, 59},
        {41, 54, 39, 42, 56},
        {40, 53, 40, 40, 58},
        {41, 53, 40, 42, 59},
        {40, 54, 41, 43, 58},
        {39, 54, 40, 42, 59},
        {40, 54, 39, 41, 60},
        {41, 55, 39, 42, 60},
        {41, 53, 40, 41, 57},
        {40, 55, 38, 40, 59},
        {41, 56, 39, 43, 58},
        {41, 56, 39, 40, 58},
        {43, 55, 39, 42, 59},
        {41, 55, 40, 42, 59},
        {41, 58, 41, 43, 58},
        {39, 55, 39, 41, 59},
        {41, 56, 43, 41, 59},
        {41, 56, 42, 44, 57},
        {40, 54, 40, 42, 57},
        {42, 54, 40, 43, 59},
        {41, 54, 41, 41, 58},
        {41, 53, 40, 46, 59},
        {43, 53, 38, 42, 59},
        {40, 56, 39, 40, 60},
        {40, 55, 39, 41, 59},
        {42, 53, 38, 41, 57},
        {40, 54, 39, 43, 59},
        {39, 54, 38, 41, 58},
        {40, 53, 41, 43, 58},
        {40, 54, 40, 41, 58},
        {43, 56, 42, 42, 58},
        {40, 54, 39, 42, 58},
        {40, 54, 38, 40, 59},
        {40, 52, 40, 40, 57},
        {38, 54, 40, 44, 58},
        {40, 53, 41, 40, 57},
        {41, 54, 39, 42, 59},
        {40, 55, 41, 40, 57},
        {42, 54, 39, 41, 58},
        {42, 55, 40, 42, 59},
        {41, 55, 40, 41, 57},
        {40, 55, 40, 43, 58},
        {41, 55, 39, 42, 58},
        {40, 52, 40, 40, 58},
        {40, 55, 40, 42, 59},
        {39, 53, 39, 43, 58},
        {41, 54, 38, 42, 59},
        {41, 53, 40, 41, 60},
        {40, 54, 40, 43, 56},
        {41, 53, 39, 42, 58},
        {42, 54, 42, 41, 57},
        {40, 55, 39, 41, 57},
        {39, 55, 40, 42, 56},
        {41, 53, 40, 41, 57}
    };

    double locations[][3] = {
        {84.01, 84.02, 3.14},    // DEBUG_LOCALIZE_MEASURE
        {84.01, 84.06, 3.14},    // DEBUG_LOCALIZE_MEASURE
        {84.07, 84.17, 3.14},    // DEBUG_LOCALIZE_MEASURE
        {84.09, 84.32, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.13, 84.33, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.17, 84.37, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.26, 84.38, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.28, 84.57, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.31, 84.64, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.30, 84.67, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.32, 84.80, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.33, 84.85, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.38, 84.87, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.41, 84.90, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.46, 84.95, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.50, 84.93, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.53, 84.92, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.54, 84.90, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.55, 84.83, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.55, 84.74, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.60, 84.79, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.57, 84.78, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.56, 84.78, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.57, 84.86, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.61, 84.97, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.64, 85.03, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.69, 85.09, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.71, 85.08, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.74, 85.06, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.74, 85.11, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.75, 85.16, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.73, 85.15, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.80, 85.08, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.84, 85.11, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.85, 85.18, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.85, 85.15, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.85, 85.10, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.83, 85.07, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.88, 85.14, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.88, 85.15, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.88, 85.16, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.90, 85.21, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.93, 85.17, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.89, 85.16, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.89, 85.18, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.89, 85.17, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.89, 85.16, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {84.90, 85.12, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.90, 85.21, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.92, 85.24, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.96, 85.24, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.96, 85.28, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.96, 85.30, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {84.98, 85.33, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {85.04, 85.31, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {85.04, 85.32, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {85.04, 85.36, 3.15},    // DEBUG_LOCALIZE_MEASURE
        {85.08, 85.35, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.08, 85.22, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.06, 85.26, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.05, 85.28, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.09, 85.34, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.11, 85.35, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.11, 85.30, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.13, 85.30, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.14, 85.34, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.18, 85.37, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.19, 85.42, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.19, 85.45, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.18, 85.45, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.20, 85.42, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.18, 85.38, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.17, 85.35, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.17, 85.26, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.17, 85.22, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.19, 85.22, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.22, 85.17, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.22, 85.19, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.29, 85.16, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.35, 85.15, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.36, 85.16, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.37, 85.18, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.40, 85.17, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.41, 85.30, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.39, 85.29, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.38, 85.27, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.37, 85.27, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.34, 85.22, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.33, 85.29, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.31, 85.30, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.34, 85.36, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.35, 85.35, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.41, 85.26, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.40, 85.28, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.37, 85.28, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.38, 85.28, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.39, 85.38, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.42, 85.35, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.41, 85.36, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.45, 85.29, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.44, 85.25, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.45, 85.23, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.46, 85.17, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.47, 85.20, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.46, 85.18, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.47, 85.21, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.48, 85.23, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.47, 85.31, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.44, 85.33, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.45, 85.36, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.46, 85.35, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.45, 85.36, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.50, 85.29, 3.17},    // DEBUG_LOCALIZE_MEASURE
        {85.49, 85.25, 3.16},    // DEBUG_LOCALIZE_MEASURE
        {85.50, 85.23, 3.16}    // DEBUG_LOCALIZE_MEASURE
    };

    sensor_reading_t sensor_test_data[NUM_SENSORS];
    sensor_test_data[0] = (sensor_reading_t){ .state = GOOD, .distance = 40 };
    sensor_test_data[1] = (sensor_reading_t){ .state = GOOD, .distance = 56  };
    sensor_test_data[2] = (sensor_reading_t){ .state = GOOD, .distance = 40 };
    sensor_test_data[3] = (sensor_reading_t){ .state = GOOD, .distance = 41 };
    sensor_test_data[4] = (sensor_reading_t){ .state = GOOD, .distance = 56 };

    robot_location.x_mu = 83.97;
    robot_location.y_mu = 84.0;
    robot_location.theta_mu = PI;

    //robot_maze_state.cells[0][0].south->exists = 1.0;
    //robot_maze_state.cells[0][1].south->exists = 1.0;

    printf("Robot_location: (%f, %f, %f)\n\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);
    
    for (int i = 0; i < 5; i++) {

        sensor_test_data[0].distance = measurements[i][0];
        sensor_test_data[1].distance = measurements[i][1];
        sensor_test_data[2].distance = measurements[i][2];
        sensor_test_data[3].distance = measurements[i][3];
        sensor_test_data[4].distance = measurements[i][4];

        mazeMappingAndMeasureStep(sensor_test_data);

        printf("Robot_location: (%f, %f, %f)\n\n", robot_location.x_mu, robot_location.y_mu, robot_location.theta_mu);
    }

    // printf("North: %f\n", robot_maze_state.cells[1][1].north->exists);
    // printf("East: %f\n", robot_maze_state.cells[1][1].east->exists);
    // printf("South: %f\n", robot_maze_state.cells[1][1].south->exists);
    // printf("West: %f\n", robot_maze_state.cells[1][1].west->exists);
    
    print_maze_state();

    //TEST_FAIL("not all tests written yet!!!");

    // Test localizeMeasureStep
    // TEST_FAIL("not all tests written yet!!!");

} TEST_FUNC_END("localization_test")

#endif // ARDUINO
