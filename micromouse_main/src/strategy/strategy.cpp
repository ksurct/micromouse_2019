/* strategy.cpp */


#include "strategy.h"
#include "../types.h"
#include "../settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <queue>


using namespace std;

/* Simple representation of a cell
 *  - For use with probabilistic_maze_t */
typedef struct {
   int x;
   int y;
} cell_t;


// Function declarations
void floodfill(probabilistic_maze_t* maze_state, cell_t cell, int value);
void convertLocationToCell(gaussian_location_t* location, cell_t* to_return);
void convertCellToLocation(cell_t* cell, gaussian_location_t* to_return);
cell_t chooseNextCell(probabilistic_maze_t* robot_maze_state, cell_t* robot_cell);
void resetValues(void);
void setAllDiscoveredToFalse(void);


// Global declarations
static cell_t goal_cell = {
    .x = GOAL_CELL_X,
    .y = GOAL_CELL_Y
};

/* The number of steps away from the goal based on the floodfill algorithm */
int values[MAZE_WIDTH][MAZE_HEIGHT];

/* Keeps track of which cells have been discovered already */
bool discovered[MAZE_WIDTH][MAZE_HEIGHT];


/* initialize strategy
 * Initializes the maze solving algorithm */
void initializeStrategy(void) {

    resetValues();
    setAllDiscoveredToFalse();
}

/* strategy
 * Given the robots location and the state of the maze calculate the next location to go to */
void strategy(gaussian_location_t* robot_location, probabilistic_maze_t* maze_state, gaussian_location_t* next_location) {

    // Get the current cell
    cell_t robot_cell;
    convertLocationToCell(robot_location, &robot_cell);

    // Update values by floodfill
    floodfill(maze_state, goal_cell, 0);

    // Choose the lowest valued cell we can go to
    cell_t next_cell = chooseNextCell(maze_state, &robot_cell);

    // Convert to a gaussian_location_t that edits the next_location
    convertCellToLocation(&next_cell, next_location);
}

#define IS_CELL_OUT_OF_BOUNDS(cell) ((cell).x < 0 || (cell).x >= (MAZE_WIDTH) || (cell).y < 0 || (cell).y >= (MAZE_HEIGHT))

/* floodfill
 * Implements the floodfill algorithm on the 2d-array values with breadth first search
 * Values should be set to numbers higher than possible to have */
void floodfill(probabilistic_maze_t* maze_state, cell_t cell, int value) {

    // Reset everything
    resetValues();
    setAllDiscoveredToFalse();

    queue<cell_t> q;
    queue<cell_t> next_q;

    discovered[cell.x][cell.y] = true;

    q.push(cell);

    while (!q.empty() || !next_q.empty()) {

        if (q.empty()) {
            swap(q, next_q);
            value++;
        }

        cell = q.front();
        q.pop();

        // Update the cell
        if (IS_CELL_OUT_OF_BOUNDS(cell)) {
            continue;
        }
        values[cell.x][cell.y] = value;

        // For each direction, east(-1, 0), north(0, -1), west(1, 0), south(0, 1)
        // if we haven't been there and we can get there, go there
        cell_t next;

        // Check North(0, -1)
        next.x = cell.x; next.y = cell.y-1;
        if (!IS_CELL_OUT_OF_BOUNDS(next)) {
            if (!discovered[next.x][next.y] && maze_state->cells[cell.x][cell.y].north->exists < WALL_THRESHOLD) {
                discovered[next.x][next.y] = true;
                next_q.push(next);
            }
        }

        // Check East(1, 0)
        next.x = cell.x+1; next.y = cell.y;
        if (!IS_CELL_OUT_OF_BOUNDS(next)) {
            if (!discovered[next.x][next.y] && maze_state->cells[cell.x][cell.y].east->exists < WALL_THRESHOLD) {
                discovered[next.x][next.y] = true;
                next_q.push(next);
            }
        }

        // Check South(0, 1)
        next.x = cell.x; next.y = cell.y+1;
        if (!IS_CELL_OUT_OF_BOUNDS(next)) {
            if (!discovered[next.x][next.y] && maze_state->cells[cell.x][cell.y].south->exists < WALL_THRESHOLD) {
                discovered[next.x][next.y] = true;
                next_q.push(next);
            }
        }

        // Check West(-1, 0)
        next.x = cell.x-1; next.y = cell.y;
        if (!IS_CELL_OUT_OF_BOUNDS(next)) {
            if (!discovered[next.x][next.y] && maze_state->cells[cell.x][cell.y].west->exists < WALL_THRESHOLD) {
                discovered[next.x][next.y] = true;
                next_q.push(next);
            }
        }
    }
    
    // // Print out the maze for debugging
    // printf("\n");
    // for (int i=0; i<MAZE_WIDTH; i++){
    //     for (int j=0; j<MAZE_HEIGHT; j++){
    //         if (values[i][j] <10){
    //             printf("  %d  |", values[i][j]);
    //         }
    //         else if (values[i][j] < 100){
    //             printf(" %d  |", values[i][j]);
    //         }
    //         else{
    //             printf(" %d |", values[i][j]);
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("\n"); 
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
    
    // Check North(0, -1)
    if (values[x][y - 1] < lowest_value && robot_maze_state->cells[x][y].north->exists < WALL_THRESHOLD) {
        // Choose North
        lowest_value = values[x][y - 1];
        next_cell.x = x;
        next_cell.y = y - 1;
    }

    // Check East(1, 0)
    if (values[x + 1][y] < lowest_value && robot_maze_state->cells[x][y].east->exists < WALL_THRESHOLD) {
        // Choose East
        lowest_value = values[x + 1][y];
        next_cell.x = x + 1;
        next_cell.y = y;
    }

    // Check South(0, 1)
    if (values[x][y + 1] < lowest_value && robot_maze_state->cells[x][y].south->exists < WALL_THRESHOLD) {
        // Choose South
        lowest_value = values[x][y + 1];
        next_cell.x = x;
        next_cell.y = y + 1;
    }

    // Check West(-1, 0)
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

void setAllDiscoveredToFalse(void){
    for (int i=0; i<MAZE_WIDTH; i++){
        for (int j=0; j<MAZE_HEIGHT; j++){
            discovered[i][j] = false;
        }
    }
}
