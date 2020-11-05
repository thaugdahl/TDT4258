#ifndef _HENRIK_TEST_H
#define _HENRIK_TEST_H

#include <stdint.h>

#define TIMEOUT 10

#define GET_DIRECTION(data, direction) (data  & (0b1 << direction))
#define SET_DIRECTION(direction) (0b1 << direction)

#define GET_WALKED_DIRECTION(data, direction) GET_DIRECTION((data >> 4), direction)
#define GET_VALID_DIRECTION(data, direction) GET_DIRECTION(data, direction)

#define SET_WALKED_DIRECTION(direction) SET_DIRECTION(direction) << 4
#define SET_VALID_DIRECTION(direction) SET_DIRECTION(direction)

#define COR_TO_INDEX(x, y, scale_x) (x+(y*scale_x))

#define VALID_PATH_MASK 0b1111
#define WALKED_PATH_MASK 0b1111<<4

enum direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

typedef uint8_t pos_t;

typedef struct
{
    uint8_t paths;  // divided into two categoris, valid and walked
                    // valid: first four bits, paths that can be tested
                    // walked: last four bits, paths that have been tested, and is valid
    uint8_t visited; // check if the node is visited
}square_t;

typedef struct actor_t
{
    pos_t x; // x cordinate
    pos_t y; // y cordinate
}actor_t;

typedef struct
{
    uint16_t size_x; //side length for x 
    uint16_t size_y; //side length for y 
    actor_t  start_pos; //x, y location for the start position
    pos_t    length_x; // number of x squares
    pos_t    length_y; // number of y squares
    square_t *squares; //each square in the maze
}maze_t;


typedef struct 
{
    uint16_t MAX;
    int head;
    actor_t *stack;
}actor_stack_t;


#endif