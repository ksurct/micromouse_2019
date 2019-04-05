#ifndef ARDUINO
#include "strategy.h"
#include "strategy_test_data.h"
#include "../testing.h"
#include "../settings.h"
#include "../types.h"


// Helper Functions
double convertCellToLocation(int x) {
    return (double) (((x) * (WALL_THICKNESS + CELL_LENGTH)) + (CELL_LENGTH / 2));
}

int convertLocationToCell(double x) {
    return (int) (x / (WALL_THICKNESS + CELL_LENGTH));
}

void readInMaze(const char** maze_string, probabilistic_maze_t* maze) {

    int i2 = 0;
    for (int i = 1; i < size; i+=2) {
        int j2 = 0;
        for (int j = 1; j < size; j+=2) {
            
            // North(0, -1)
            if (maze_string[i][j-1] == 'X') maze->cells[i2][j2].north->exists = 1.0;
            else maze->cells[i2][j2].north->exists = 0.0;
            
            // East(1, 0)
            if (maze_string[i+1][j] == 'X') maze->cells[i2][j2].east->exists = 1.0;
            else maze->cells[i2][j2].east->exists = 0.0;
            
            // South(0, 1)
            if (maze_string[i][j+1] == 'X') maze->cells[i2][j2].south->exists = 1.0;
            else maze->cells[i2][j2].south->exists = 0.0;
            
            // West(-1, 0)
            if (maze_string[i-1][j] == 'X') maze->cells[i2][j2].west->exists = 1.0;
            else maze->cells[i2][j2].west->exists = 0.0;
            
            j2++;
        }
        i2++;
    }
}


TEST_FUNC_BEGIN {
    
    initializeStrategy();
    TEST_PASS("initializeStrategy called");

    gaussian_location_t location;
    gaussian_location_t location2;
    probabilistic_maze_t robot_maze;
    initializeMaze(&robot_maze);
    int MAX_STEPS;
    int steps;

    // Test empty maze
    MAX_STEPS = MAZE_SIZE-1;
    steps = 0;

    // Setup initial location
    location.x_mu = INIT_X_MU;
    location.y_mu = INIT_Y_MU;

    // Read in the maze
    readInMaze(empty_string, &robot_maze);

    // While not at goal
    while (location.x_mu != (convertCellToLocation(GOAL_CELL_X)) ||
            location.y_mu != (convertCellToLocation(GOAL_CELL_Y)))
    {
        // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

        // Run strategy
        strategy(&location, &robot_maze, &location2);
        
        // Update location
        location = location2;

        if (++steps >= MAX_STEPS) {
            TEST_FAIL("Empty maze solve");
            goto after_empty_solve;
        }
    }

    // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

    TEST_PASS("Empty maze solve");

    // printf("\t took %d steps\n", steps);
    after_empty_solve:
    ;

    // Test spiral maze
    MAX_STEPS = 500;
    steps = 0;

    // Setup initial location
    location.x_mu = INIT_X_MU;
    location.y_mu = INIT_Y_MU;

    // Read in the maze
    readInMaze(spiral_string, &robot_maze);

    // While not at goal
    while (location.x_mu != (convertCellToLocation(GOAL_CELL_X)) ||
            location.y_mu != (convertCellToLocation(GOAL_CELL_Y)))
    {
        // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

        // Run strategy
        strategy(&location, &robot_maze, &location2);
        
        // Update location
        location = location2;

        if (++steps >= MAX_STEPS) {
            TEST_FAIL("Spiral maze solve");
            goto after_spiral_solve;
        }
    }

    // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

    TEST_PASS("Spiral maze solve");

    // printf("\t took %d steps\n", steps);
    after_spiral_solve:
    ;

    // Test loop maze
    MAX_STEPS = 500;
    steps = 0;

    // Setup initial location
    location.x_mu = INIT_X_MU;
    location.y_mu = INIT_Y_MU;

    // Read in the maze
    readInMaze(loop_string, &robot_maze);

    // While not at goal
    while (location.x_mu != (convertCellToLocation(GOAL_CELL_X)) ||
            location.y_mu != (convertCellToLocation(GOAL_CELL_Y)))
    {
        // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

        // Run strategy
        strategy(&location, &robot_maze, &location2);
        
        // Update location
        location = location2;

        if (++steps >= MAX_STEPS) {
            TEST_FAIL("Loop maze solve");
            goto after_loop_solve;
        }
    }

    // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

    TEST_PASS("Loop maze solve");

    // printf("\t took %d steps\n", steps);
    after_loop_solve:
    ;

    // Test actual maze
    MAX_STEPS = 500;
    steps = 0;

    // Setup initial location
    location.x_mu = INIT_X_MU;
    location.y_mu = INIT_Y_MU;

    // Read in the maze
    readInMaze(actual_string, &robot_maze);

    // While not at goal
    while (location.x_mu != (convertCellToLocation(GOAL_CELL_X)) ||
            location.y_mu != (convertCellToLocation(GOAL_CELL_Y)))
    {
        // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

        // Run strategy
        strategy(&location, &robot_maze, &location2);
        
        // Update location
        location = location2;

        if (++steps >= MAX_STEPS) {
            TEST_FAIL("Actual maze solve");
            goto after_actual_solve;
        }
    }

    // printf("Step: %d,\tX: %d,\tY: %d\n", steps, convertLocationToCell(location.x_mu), convertLocationToCell(location.y_mu));

    TEST_PASS("Actual maze solve");

    // printf("\t took %d steps\n", steps);
    after_actual_solve:
    ;

} TEST_FUNC_END("strategy_test")

#endif // ARDUINO
