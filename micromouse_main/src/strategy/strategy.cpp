/* strategy.cpp */


#include "strategy.h"
#include "../types.h"
#include "../settings.h"


/* Simple representation of a cell 
 *  - For use with probabilistic_maze_t */
typedef struct {
   int x;
   int y; 
} cell_t;


// Function declarations
void floodfill(probabilistic_maze_t* robot_maze_state, cell_t* cell, int value);
void convertLocationToCell(gaussian_location_t* location, cell_t* to_return);
void convertCellToLocation(cell_t* cell, gaussian_location_t* to_return);
cell_t chooseNextCell(probabilistic_maze_t* robot_maze_state, cell_t* robot_cell);
void resetValues(void);


// Global declarations
static cell_t goal_cell = {
    .x = GOAL_CELL_X,
    .y = GOAL_CELL_Y
};

/* The number of steps away from the goal based on the floodfill algorithm */
int values[MAZE_WIDTH][MAZE_HEIGHT];


/* initialize strategy
 * Initializes the maze solving algorithm */
void initializeStrategy(void) {
    
    resetValues();
}

/* strategy
 * Given the robots location and the state of the maze calculate the next location to go to */
void strategy(gaussian_location_t* robot_location, probabilistic_maze_t* robot_maze_state, gaussian_location_t* next_location) {
    
    // Get the current cell
    cell_t robot_cell;
    convertLocationToCell(robot_location, &robot_cell);

    // Rest values to MAX_VALUE
    resetValues();

    // Update values by floodfill
    floodfill(robot_maze_state, &goal_cell, 0);

    // Choose the lowest valued cell we can go to
    cell_t next_cell = chooseNextCell(robot_maze_state, &robot_cell);

    // Convert to a gaussian_location_t that edits the next_location
    convertCellToLocation(&next_cell, next_location);
}

/* floodfill
 * Implements the floodfill algorithm on the 2d-array values
 * Values should be set to numbers higher than possible to have */
void floodfill(probabilistic_maze_t* robot_maze_state, cell_t* cell, int value) {
    
    // base case, outside of maze (shouldn't ever happen)
    // remove if it is guaranteed that all outside walls will be > WALL_THRESHOLD
    if (cell->x < 0 || cell->x >= MAZE_WIDTH || cell->y < 0 || cell->y >= MAZE_HEIGHT) {
        return;
    }

    // We are at an actual cell, give it the current value if it lowers it
    if (value < values[cell->x][cell->y])
        values[cell->x][cell->y] = value;

    // Check each direction from this cell and floodfill it
    
    // Check North
    if (robot_maze_state->cells[cell->x][cell->y].north->exists < WALL_THRESHOLD) {
        cell->y--; // North one
        floodfill(robot_maze_state, cell, value + 1);
        cell->y++;
    }

    // Check East
    if (robot_maze_state->cells[cell->x][cell->y].east->exists < WALL_THRESHOLD) {
        cell->x++; // East one
        floodfill(robot_maze_state, cell, value + 1);
        cell->x--;
    }

    // Check South
    if (robot_maze_state->cells[cell->x][cell->y].south->exists < WALL_THRESHOLD) {
        cell->y++; // South one
        floodfill(robot_maze_state, cell, value + 1);
        cell->y--;
    }
    
    // Check West
    if (robot_maze_state->cells[cell->x][cell->y].west->exists < WALL_THRESHOLD) {
        cell->x--; // West one
        floodfill(robot_maze_state, cell, value + 1);
        cell->x++;
    }
}

/* Uses the mean location to determine the cell this location is in */
void convertLocationToCell(gaussian_location_t* location, cell_t* to_return) {
    to_return->x = (int) (location->x.mean / (WALL_THICKNESS + CELL_LENGTH));
    to_return->y = (int) (location->y.mean / (WALL_THICKNESS + CELL_LENGTH));
}

/* Returns the middle of the cell given only changing the x and y mean */
void convertCellToLocation(cell_t* cell, gaussian_location_t* to_return) {
    to_return->x.mean = (double) (((cell->x) * (WALL_THICKNESS + CELL_LENGTH)) + (CELL_LENGTH / 2));
    to_return->y.mean = (double) (((cell->y) * (WALL_THICKNESS + CELL_LENGTH)) + (CELL_LENGTH / 2));
}

/* Return the next cell that we can go to with the lowest value */
cell_t chooseNextCell(probabilistic_maze_t* robot_maze_state, cell_t* robot_cell) {

    int x = robot_cell->x;
    int y = robot_cell->y;
    int lowest_value = MAX_VALUE;
    cell_t next_cell;

    // Check each direction and save the lowest valued direction that we can go to

    // Check North
    if (values[x][y - 1] < lowest_value && robot_maze_state->cells[x][y].north->exists < WALL_THRESHOLD) {
        // Choose North
        lowest_value = values[x][y - 1];
        next_cell.x = x;
        next_cell.y = y - 1;
    }

    // Check East
    if (values[x + 1][y] < lowest_value && robot_maze_state->cells[x][y].east->exists < WALL_THRESHOLD) {
        // Choose East
        lowest_value = values[x + 1][y];
        next_cell.x = x + 1;
        next_cell.y = y;
    }

    // Check South
    if (values[x][y + 1] < lowest_value && robot_maze_state->cells[x][y].south->exists < WALL_THRESHOLD) {
        // Choose South
        lowest_value = values[x][y + 1];
        next_cell.x = x;
        next_cell.y = y + 1;
    }

    // Check West
    if (values[x - 1][y] < lowest_value && robot_maze_state->cells[x][y].west->exists < WALL_THRESHOLD) {
        // Choose West
        lowest_value = values[x - 1][y];
        next_cell.x = x - 1;
        next_cell.y = y;
    }
    
    return next_cell;
}

/* Resets Values 2d-array back MAX_VALUE */
void resetValues(void) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
        for (int y = 0; y < MAZE_HEIGHT; y++) {
            values[x][y] = MAX_VALUE;
        }
    }
}
