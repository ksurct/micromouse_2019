/* strategy.cpp */


#include "strategy.h"
#include "../types.h"
#include "../settings.h"


typedef struct {
   int x;
   int y; 
} cell_t;


// Function declarations
void floodfill(probabilistic_maze_t* robot_maze_state);



/* The number of steps away from the goal based on the floodfill algorithm */
int values[MAZE_WIDTH][MAZE_HEIGHT];


/* initialize strategy
 * Initializes the maze solving algorithm */
void initializeStrategy(void) {
    // dothething();
}

/* strategy
 * Given the robots location and the state of the maze calculate the next location to go to */
gaussian_location_t* strategy(gaussian_location_t* robot_location, probabilistic_maze_t* robot_maze_state) {
    // dothething();
}

/* floodfill
 * Implements the floodfill algorithm on the 2d-array values */
void floodfill(probabilistic_maze_t* robot_maze_state, cell_t* cell) {
    // dothething();
}
