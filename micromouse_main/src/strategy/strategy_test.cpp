#ifndef ARDUINO
#include "strategy.h"
#include "../testing.h"
#include "../settings.h"
#include "../types.h"


TEST_FUNC_BEGIN {
    
    // Setup a maze to test with
    probabilistic_maze_t maze;
    initializeMaze(&maze);

    // Initialize a location to test with
    gaussian_location_t location;
    location.x.mean = 100.0f;
    location.x.sigma2 = 10.0f;
    location.y.mean = 100.0f;
    location.y.sigma2 = 10.0f;
    location.theta.mean = 0.0f;
    location.theta.sigma2 = 10.0f;


    // Setup strategy
    initializeStrategy();
    

    // 



} TEST_FUNC_END("strategy_test")

#endif // ARDUINO
