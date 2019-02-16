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
 * Implements the floodfill algorithm on the 2d-array values
 * Values should be set to numbers higher than possible to have */
void floodfill(probabilistic_maze_t* robot_maze_state, cell_t* cell, int value) {
    
    // base case, outside of maze (shouldn't ever happen)
    // remove if it is garenteed that all outside walls will be > WALL_THRESHOLD
    if (cell->x < 0 || cell->x >= MAZE_WIDTH || cell->y < 0 || cell->y >= MAZE_HEIGHT) {
        return;
    }

    // We are at an actual cell, give it the current value if it lowers it
    if (value < values[cell->x][cell->y])
        values[cell->x][cell->y] = value;

    // Check each direction from this cell and floodfill it
    if (robot_maze_state->cells[cell->x][cell->y].north->exists < WALL_THRESHOLD) {
        cell->y--; // North one
        floodfill(robot_maze_state, cell, value + 1);
        cell->y++;
    }
    // Check each direction from this cell and floodfill it
    if (robot_maze_state->cells[cell->x][cell->y].east->exists < WALL_THRESHOLD) {
        cell->x++; // East one
        floodfill(robot_maze_state, cell, value + 1);
        cell->x--;
    }
    // Check each direction from this cell and floodfill it
    if (robot_maze_state->cells[cell->x][cell->y].south->exists < WALL_THRESHOLD) {
        cell->y++; // South one
        floodfill(robot_maze_state, cell, value + 1);
        cell->y--;
    }
    // Check each direction from this cell and floodfill it
    if (robot_maze_state->cells[cell->x][cell->y].west->exists < WALL_THRESHOLD) {
        cell->x--; // West one
        floodfill(robot_maze_state, cell, value + 1);
        cell->x++;
    }
}
