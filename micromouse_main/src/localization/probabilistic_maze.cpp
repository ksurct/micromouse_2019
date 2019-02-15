/* probabilistic_maze.cpp */

#include <string.h>
#include "probabilistic_maze.h"
#include "../settings.h"

/* Initalize the state of the maze
 *   - Responsible for setting up the pointers to the
 * wall in such a way that there are no duplicates.
 */
void initalizeMaze(probabilistic_maze_t* maze) {
    for (int i = 0; i < MAZE_HEIGHT * (MAZE_WIDTH + 1) + (MAZE_HEIGHT + 1) * MAZE_WIDTH; ++i) {
        maze->wall_buffer[i].exists = 0.5;
    }
    for (int x = 0; x < MAZE_WIDTH; ++x) {
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            maze->cells[x][y] = {
                .north = &maze->wall_buffer[x * MAZE_WIDTH + y],
                .east = &maze->wall_buffer[MAZE_HEIGHT * (MAZE_WIDTH + 1) + x * MAZE_WIDTH + y],
                .south = &maze->wall_buffer[x * MAZE_WIDTH + (y + 1)],
                .west = &maze->wall_buffer[MAZE_HEIGHT * (MAZE_WIDTH + 1) + (x + 1) * MAZE_WIDTH + y]
            };
            if (y == 0) {
                maze->cells[x][y].north->exists = 1;
            } else if (y == MAZE_HEIGHT - 1) {
                maze->cells[x][y].south->exists = 1;
            }
            if (x == 0) {
                maze->cells[x][y].east->exists = 1;
            } else if (x == MAZE_WIDTH - 1) {
                maze->cells[x][y].west->exists = 1;
            }
        }
    }
}
