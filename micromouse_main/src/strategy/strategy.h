/* strategy.h */


#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "../types.h"
#include "../localization/probabilistic_maze.h"

/* initialize strategy
 * Initializes the maze solving algorithm */
void initializeStrategy(void);

/* strategy
 * Given the robots location and the state of the maze calculate the next location to go to */
gaussian_location_t* strategy(gaussian_location_t* robot_location, probabilistic_maze_t* robot_maze_state);


#endif //_STRATEGY_H_
