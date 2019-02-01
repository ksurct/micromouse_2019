/* probabilistic_maze.h
 *  
 *  This file defines a probabilistic maze where each
 *  wall holds the probablity of that wall exsits.
 */



#include "settings.h"


typedef struct {
    double exsits;
} probabilistic_wall_t;


typedef struct {
    probabilistic_wall_t * north;
    probabilistic_wall_t * east;
    probabilistic_wall_t * south;
    probabilistic_wall_t * west;
} probabilistic_cell_t;


typedef struct {
    probabilistic_cell_t cells[MAZE_WIDTH][MAZE_HEIGHT];
} probabilistic_maze_t;


/* Initalize the state of the maze
 *   - Responsible for setting up the pointers to the
 * wall in such a way that there are no duplicates.
 */
void initalizeMaze(probabilistic_maze_t* maze);
