#ifndef _HENRIK_TEST_H
#define _HENRIK_TEST_H

#include <stdint.h>

#include <linux/fb.h>

#include "game.h"

#define TIMEOUT 10
#define MIN_SIZE_IN_PIXEL 4

#define GET_DIRECTION(data, direction) (data  & (0b1 << direction))
#define SET_DIRECTION(direction) (0b1 << direction)

#define GET_WALKED_DIRECTION(data, direction) GET_DIRECTION((data >> 4), direction)

#define SET_WALKED_DIRECTION(direction) (SET_DIRECTION(direction) << 4)

#define SET_VISITED(data)   (data |=  0b1)
#define CLEAR_VISITED(data) (data &= ~0b1)
#define GET_VISITED(data)   (data &   0b1)


#define SET_DONE(data)   (data |=  (1 << 0b1))
#define CLEAR_DONE(data) (data &= ~(1 << 0b1))
#define GET_DONE(data)   (data &   (1 << 0b1))


#define COR_TO_INDEX(x, y, scale_x) (x+(y*scale_x))

#define WALKED_PATH_MASK 0b1111<<4
#define DONE_MASK 0b10
#define VISITED_MASK 0b1

typedef uint8_t pos_t;

typedef uint8_t direction_t;

enum direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

/**
 * divided into three categoris, visited, done and walked
 * visited: first bit, checks if the node has been visited
 * done: second bit, tells the system the node is complete
 * walked: last four bits, paths that have been tested, and is valid
 */
typedef uint8_t square_t;

typedef struct actor_t
{
    pos_t x; // x cordinate
    pos_t y; // y cordinate
}actor_t;

typedef struct
{
    uint16_t size_x; //side length for x 
    uint16_t size_y; //side length for y 
    uint16_t actor_size_x; //x side size for the actor
    uint16_t actor_size_y; //y side size for the actor
    actor_t  start_pos; //x, y location for the start position
    actor_t  end_pos; //x, y location for the goal
    pos_t    length_x; //number of x squares
    pos_t    length_y; //number of y squares
    square_t *squares; //each square in the maze
    uint16_t BG_color; //the background color for the maze
    uint16_t actor_color; //the actor color
    uint16_t wall_color; //the wall color for the maze
    uint16_t goal_color; // the color of the goal in game
}maze_t;


typedef struct 
{
    uint16_t MAX; //max length of the stack
    int head; // the current top index of the stack
    actor_t *stack; //the stack array
}actor_stack_t;


void write_maze_to_screenvalues(maze_t *maze, 
                                uint16_t *screen_values,
                                uint16_t screen_length);

void generate_maze( pos_t    squares_x,
                    pos_t    squares_y,
                    uint16_t size_x,
                    uint16_t size_y,
                    uint16_t actor_size_x,
                    uint16_t actor_size_y,
                    maze_t   *maze,
                    pos_t    start_x,
                    pos_t    start_y,
                    pos_t    end_x,
                    pos_t    end_y,
                    uint16_t BG_color,
                    uint16_t actor_color,
                    uint16_t wall_color,
                    uint16_t goal_color);

void clean_maze(maze_t *maze);

void init_maze( maze_t * maze,
                uint16_t squares_x,
                uint16_t squares_y);

void _move_actor_ignore_walls_(actor_t *actor,
                               maze_t *maze, 
                               direction_t direction,
                               uint16_t *screen_values,
                               uint16_t screen_length,
                               int fbfd);

int move_actor( actor_t *actor,
                maze_t *maze, 
                direction_t direction,
                uint16_t *screen_values,
                uint16_t screen_length,
                int fbfd);

void draw_goal( maze_t * maze,
                uint16_t *screen_values,
                uint16_t screen_length_x);

#endif