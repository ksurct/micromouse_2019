#ifndef ARDUINO
#include "probabilistic_maze.h"
#include "../testing.h"

int count_cell_references(probabilistic_maze_t *maze, probabilistic_wall_t *check) {
    int count = 0;
    for (int x = 0; x < MAZE_WIDTH; ++x) {
        for (int y = 0; y < MAZE_HEIGHT; ++y) {
            count += (maze->cells[x][y].north == check) +
                     (maze->cells[x][y].east == check) +
                     (maze->cells[x][y].south == check) +
                     (maze->cells[x][y].west == check);
        }
    }
    return count;
}

TEST_FUNC_BEGIN {
    probabilistic_maze_t maze;
    initializeMaze(&maze);
    TEST_PASS("initializeMaze called");

    for (int x = 0; x < MAZE_WIDTH; ++x) {
        if (maze.cells[x][0].north->exists != 1) {
            TEST_FAIL("north border is 1");
            goto after_north_border;
        }
    }
    TEST_PASS("north border is 1");
    after_north_border:

    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        if (maze.cells[MAZE_WIDTH - 1][y].east->exists != 1) {
            TEST_FAIL("east border is 1");
            goto after_east_border;
        }
    }
    TEST_PASS("east border is 1");
    after_east_border:

    for (int x = 0; x < MAZE_WIDTH; ++x) {
        if (maze.cells[x][MAZE_HEIGHT - 1].south->exists != 1) {
            TEST_FAIL("south border is 1");
            goto after_south_border;
        }
    }
    TEST_PASS("south border is 1");
    after_south_border:

    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        if (maze.cells[0][y].west->exists != 1) {
            TEST_FAIL("west border is 1");
            goto after_west_border;
        }
    }
    TEST_PASS("west border is 1");
    after_west_border:

    for (int x = 1; x < MAZE_WIDTH - 1; ++x) {
        for (int y = 1; y < MAZE_HEIGHT - 1; ++y) {
            if (maze.cells[x][y].north->exists != 0.5 || maze.cells[x][y].south->exists != 0.5 || maze.cells[x][y].east->exists != 0.5 || maze.cells[x][y].west->exists != 0.5) {
                TEST_FAIL("interior borders are 0.5");
                goto after_interior_borders;
            }
        }
    }
    for (int x = 1; x < MAZE_WIDTH - 1; ++x) {
        if (maze.cells[x][0].south->exists != 0.5 || maze.cells[x][0].east->exists != 0.5 || maze.cells[x][0].west->exists != 0.5) {
            TEST_FAIL("interior borders are 0.5");
            goto after_interior_borders;
        }
        if (maze.cells[x][MAZE_HEIGHT - 1].north->exists != 0.5 || maze.cells[x][MAZE_HEIGHT - 1].east->exists != 0.5 || maze.cells[x][MAZE_HEIGHT - 1].west->exists != 0.5) {
            TEST_FAIL("interior borders are 0.5");
            goto after_interior_borders;
        }
    }
    for (int y = 1; y < MAZE_WIDTH - 1; ++y) {
        if (maze.cells[0][y].east->exists != 0.5 || maze.cells[0][y].north->exists != 0.5 || maze.cells[0][y].south->exists != 0.5) {
            TEST_FAIL("interior borders are 0.5");
            goto after_interior_borders;
        }
        if (maze.cells[MAZE_WIDTH - 1][y].west->exists != 0.5 || maze.cells[MAZE_WIDTH - 1][y].north->exists != 0.5 || maze.cells[MAZE_WIDTH - 1][y].south->exists != 0.5) {
            TEST_FAIL("interior borders are 0.5");
            goto after_interior_borders;
        }
    }
    if (maze.cells[0][0].south->exists != 0.5 || maze.cells[0][0].east->exists != 0.5) {
        TEST_FAIL("interior borders are 0.5");
        goto after_interior_borders;
    }
    if (maze.cells[0][MAZE_HEIGHT - 1].north->exists != 0.5 || maze.cells[0][MAZE_HEIGHT - 1].east->exists != 0.5) {
        TEST_FAIL("interior borders are 0.5");
        goto after_interior_borders;
    }
    if (maze.cells[MAZE_WIDTH - 1][0].south->exists != 0.5 || maze.cells[MAZE_WIDTH - 1][0].west->exists != 0.5) {
        TEST_FAIL("interior borders are 0.5");
        goto after_interior_borders;
    }
    if (maze.cells[MAZE_WIDTH - 1][MAZE_HEIGHT - 1].north->exists != 0.5 || maze.cells[MAZE_WIDTH - 1][MAZE_HEIGHT - 1].west->exists != 0.5) {
        TEST_FAIL("interior borders are 0.5");
        goto after_interior_borders;
    }
    TEST_PASS("interior borders are 0.5");
    after_interior_borders:

    for (int i = 0; i < sizeof(maze.wall_buffer) / sizeof(maze.wall_buffer[0]); ++i) {
        if (count_cell_references(&maze, maze.wall_buffer + i) > 2) {
            TEST_FAIL("duplicated wall references");
            goto after_duplicated_walls;
        }
    }
    TEST_PASS("duplicated wall references");
    after_duplicated_walls:

    for (int x = 1; x < MAZE_WIDTH - 1; ++x) {
        for (int y = 1; y < MAZE_HEIGHT - 1; ++y) {
            if (maze.cells[x][y].north != maze.cells[x][y - 1].south ||
                maze.cells[x][y].east != maze.cells[x + 1][y].west ||
                maze.cells[x][y].south != maze.cells[x][y + 1].north ||
                maze.cells[x][y].west != maze.cells[x - 1][y].east) {
                    TEST_FAIL("invalid wall references");
                    goto after_valid_walls;
                }
        }
    }
    TEST_PASS("invalid wall references");
    after_valid_walls:
    for (int x = 0; x < MAZE_WIDTH; ++x) {
        if (count_cell_references(&maze, maze.cells[x][0].north) != 1) {
            puts("---");
            printf("%d, %d\n", x, count_cell_references(&maze, maze.cells[x][0].north));
            TEST_FAIL("invalid outer wall references");
            goto after_valid_outer_walls;
        }
        if (count_cell_references(&maze, maze.cells[x][MAZE_HEIGHT - 1].south) != 1) {
            TEST_FAIL("invalid outer wall references");
            goto after_valid_outer_walls;
        }
    }
    for (int y = 0; y < MAZE_WIDTH; ++y) {
        if (count_cell_references(&maze, maze.cells[0][y].west) != 1) {
            printf("%d\n", y);
            TEST_FAIL("invalid outer wall references");
            goto after_valid_outer_walls;
        }
        if (count_cell_references(&maze, maze.cells[MAZE_WIDTH - 1][y].east) != 1) {
            TEST_FAIL("invalid outer wall references");
            goto after_valid_outer_walls;
        }
    }
    TEST_PASS("invalid outer wall references");
    after_valid_outer_walls:

    for (int i = 0; i < sizeof(maze.wall_buffer) / sizeof(maze.wall_buffer[0]); ++i) {
        if (count_cell_references(&maze, maze.wall_buffer + i) == 0) {
            TEST_FAIL("memory waste");
            goto after_memory_waste;
        }
    }
    TEST_PASS("memory waste");
    after_memory_waste:

    asm("nop;");
} TEST_FUNC_END("probabilistic_maze_test")

#endif // ARDUINO
