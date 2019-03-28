#ifndef ARDUINO
#include "strategy.h"
#include "../testing.h"
#include "../settings.h"
#include "../types.h"


TEST_FUNC_BEGIN {
    initializeStrategy();

    gaussian_location_t robot_location;
    robot_location.x.mean = 0.0;
    robot_location.y.mean = 0.0;
    robot_location.theta.mean = PI/2;

    gaussian_location_t next_location;
    next_location.x.mean = 1.0;
    next_location.y.mean = 0.0;
    next_location.theta.mean = 0.0;

    probabilistic_maze_t robot_maze_state;
    initializeMaze(&robot_maze_state);

    while ((next_location.x.mean != 7.0) || (next_location.y.mean != 7.0))
    {
        printf("Hello there\n");
        printf("%f, %f\n",robot_location.x.mean, robot_location.y.mean);
        strategy(&robot_location, &robot_maze_state, &next_location);
        printf("General Kenobi\n");
        robot_location = next_location;
    }


    TEST_FAIL("no tests written yet!!!");



} TEST_FUNC_END("strategy_test")

#endif // ARDUINO
