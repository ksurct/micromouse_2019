#ifndef ARDUINO
#include "localization.h"
#include "../testing.h"
#include "../settings.h"


TEST_FUNC_BEGIN {
    
    initializeLocalization();
    TEST_PASS("initializeLocalization called");

    // Check the global robot_location is setup correctly
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
    // Test localizeMotionStep
    // TEST_FAIL("not all tests written yet!!!");

    // Test mazeMapping
    // TEST_FAIL("not all tests written yet!!!");

    // Test localizeMeasureStep
    // TEST_FAIL("not all tests written yet!!!");

} TEST_FUNC_END("localization_test")

#endif // ARDUINO
