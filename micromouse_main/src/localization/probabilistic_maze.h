/* probabilistic_maze.h
 *  
 *  This file defines a probabilistic maze where each
 *  wall holds the probablity of that wall exsits.
 * 
 * Structure of overall maze:
 * 
 *              maze
 *           /   |    \
 *       cell cell... cell
 *       / \   / \    / \                   
 *    wall  wall  wall   wall
 * 
 * Note: two cells will point to the same wall if the
 * two cells are adjacent.
 * 
 * --------------------------------------------------
 * 
 * Structure of cells [row, column]:
 * 
 *  [0,0][0,1] ... [0,W]
 *  [1,0][1,1] ... [1,W]
 *   ...  ...  ...  ...
 *  [H,0][H,1] ... [H,W]
 * 
 * Note: H = MAZE_HEIGHT
 *       W = MAZE_WIDTH
 */

#ifndef _PROBABILISTIC_MAZE_H_
#define _PROBABILISTIC_MAZE_H_

#include "../settings.h"


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
    probabilistic_cell_t cells[MAZE_HEIGHT][MAZE_WIDTH];
} probabilistic_maze_t;


/* Initalize the state of the maze
 *   - Responsible for setting up the pointers to the
 * wall in such a way that there are no duplicates.
 */
void initalizeMaze(probabilistic_maze_t* maze);


#endif //_PROBABILISTIC_MAZE_H_
