#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <linux/fb.h>
#include <time.h>

#include "display.h"
#include "maze.h"

void stack_push(actor_stack_t *stack, actor_t value)
{
    if (stack->head < stack->MAX)
    {
        stack->head ++;
        stack->stack[stack->head] = value;
    }else
    {
        printf("\n\n!!MAX REACH!!!!!!!\n\n\n");
    }

}

actor_t stack_pop(actor_stack_t *stack)
{
    if (stack->head >= 0)
    {
        actor_t actor;
        actor = stack->stack[stack->head];
        stack->head --;
        return actor;
    }
    else
    {
        printf("\n\n!!none left to REACH!!!!!!!\n\n\n");
        actor_t a;
        a.x = 0;
        a.y = 0;
        return a;
    }
}

void stack_clear(actor_stack_t *stack, uint16_t top)
{
    printf("ops\n");
}

void stack_create(actor_stack_t *stack, uint16_t top)
{
    stack->head = -1;
    stack->MAX = top;
    stack->stack = malloc(top*sizeof(actor_t));
    if(stack->stack == NULL)
    {
        printf("stack creation failed\n");
        exit(EXIT_FAILURE);
    }
}

void stack_delete(actor_stack_t *stack)
{
    free(stack->stack);
}

int stack_length(actor_stack_t *stack)
{
    return stack->head + 1;
}

actor_t stack_read_top(actor_stack_t *stack)
{
    return stack->stack[stack->head];
}

actor_t translate_pos(actor_t actor, direction_t direction)
{
    actor_t new_actor;
    switch (direction)
    {
    case UP:
        new_actor.x = actor.x;
        new_actor.y = actor.y - 1;
        break;

    case RIGHT:
        new_actor.x = actor.x + 1;
        new_actor.y = actor.y;
        break;

    case DOWN:
        new_actor.x = actor.x;
        new_actor.y = actor.y + 1;
        break;

    case LEFT:
        new_actor.x = actor.x - 1;
        new_actor.y = actor.y;
        break;

    default:
        new_actor.x = actor.x;
        new_actor.y = actor.y;
        break;
    }
    return new_actor;
}

uint8_t pos_to_dir(actor_t last_actor, actor_t next_actor)
{
    int dx = next_actor.x - last_actor.x;
    int dy = next_actor.y - last_actor.y;   

    if (dx == -1)
    {
        return LEFT;
    }else if (dx == 1)
    {
        return RIGHT;
    }else if (dy == -1)
    {
        return UP;
    }else
    {
        return DOWN;
    }
}

void clean_maze(maze_t *maze)
{
    uint8_t index;
    pos_t y;
    for (y = 0; y < maze->length_y; y++)
    {
        pos_t x;
        for ( x = 0; x < maze->length_x; x++)
        {
            index = COR_TO_INDEX(x, y, maze->length_x);
            maze->squares[index] = 0;
        }
    }
    SET_VISITED(maze->squares[COR_TO_INDEX(maze->start_pos.x, maze->start_pos.y, maze->length_x)]);
}
 
void _debug_print(maze_t *maze)
{
    printf("------------------------\n------------------------\n------------------------\n");
    uint32_t controll = 0;
    int y;
    for (y = 0; y < maze->length_y; y++)
    {
        int x;
        for (x = 0; x < maze->length_x; x++)
        {
            uint8_t visited = GET_VISITED(maze->squares[COR_TO_INDEX(x,y,maze->length_x)]);
        //    printf("x:%d y:%d v:%d i:%d \n",
        //           x,y,visited&1,COR_TO_INDEX(x,y,maze->length_x));
            if ((visited&0x1) == 1)
            {
                controll ++;
            }
        }
    }
    printf("visited:%d, ideal:%d", controll,maze->length_x*maze->length_y);
}

int goto_next_square( maze_t *maze,
                      actor_stack_t *path_stack)
{
    actor_stack_t new_path_stack;
    stack_create(&new_path_stack, 4);
    //uint8_t index = COR_TO_INDEX(stack_read_top(path_stack).x,
    //                             stack_read_top(path_stack).y,
    //                             maze->length_x);
    uint8_t test_index;
    actor_t next_square;
    actor_t test_pos;
    
    // test up
    test_pos = translate_pos(stack_read_top(path_stack), UP);
    test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
    if ((GET_VISITED(maze->squares[test_index]) == 0) &&
        (test_pos.x < maze->length_x) &&
        (test_pos.y < maze->length_y))
    {
        stack_push(&new_path_stack, test_pos);
    }

    // test right
    test_pos = translate_pos(stack_read_top(path_stack), RIGHT);
    test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
    if ((GET_VISITED(maze->squares[test_index]) == 0) &&
        (test_pos.x < maze->length_x) &&
        (test_pos.y < maze->length_y))
    {
        stack_push(&new_path_stack, test_pos);
    }

    // test down
    test_pos = translate_pos(stack_read_top(path_stack), DOWN);
    test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
    if ((GET_VISITED(maze->squares[test_index]) == 0) &&
        (test_pos.x < maze->length_x) &&
        (test_pos.y < maze->length_y))
    {
        stack_push(&new_path_stack, test_pos);
    }

    // test left
    test_pos = translate_pos(stack_read_top(path_stack), LEFT);
    test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
    if ((GET_VISITED(maze->squares[test_index]) == 0) &&
        (test_pos.x < maze->length_x) &&
        (test_pos.y < maze->length_y))
    {
        stack_push(&new_path_stack, test_pos);
    }

    //check len of stack, if 0 then go back
    if (stack_length(&new_path_stack) > 0)
    {
        if (stack_length(&new_path_stack) == 1)
        {
            next_square = stack_pop(&new_path_stack);
        }else
        {
            // choose the next square randomly
            int new_path_index;
            new_path_index = rand() % (stack_length(&new_path_stack));
            int i;
            for (i = 0; i < new_path_index; i++)
            {
                stack_pop(&new_path_stack);
            }
            next_square = stack_pop(&new_path_stack);
        }
    }else
    {
        // no valid path to take, so go one square back and check
        if (stack_length(path_stack) > 1)
        {
            stack_pop(path_stack);
            next_square = stack_pop(path_stack);
        }else
        {
            return 0;
        }
    }

    if (((next_square.x == stack_read_top(path_stack).x) &&
         (next_square.y == stack_read_top(path_stack).y)) ||
        ((next_square.x >= maze->length_y) &&
         (next_square.y >= maze->length_y)))
    {
        printf("!! ERROR: next path is not valid\n");
        printf("x:%d y:%d len:%d\n", stack_read_top(path_stack).x,
               stack_read_top(path_stack).y, path_stack->head);

        exit(EXIT_FAILURE);
    }

    // update walked info
    uint8_t direction_to_new = pos_to_dir(stack_read_top(path_stack), next_square);
    uint8_t direction_from_new = pos_to_dir(next_square, stack_read_top(path_stack));

    pos_t last_index = COR_TO_INDEX(stack_read_top(path_stack).x, stack_read_top(path_stack).y, maze->length_x);
    pos_t next_index = COR_TO_INDEX(next_square.x, next_square.y, maze->length_x);

    maze->squares[last_index] |= SET_WALKED_DIRECTION(direction_to_new);
    maze->squares[next_index] |= SET_WALKED_DIRECTION(direction_from_new);

    SET_VISITED(maze->squares[next_index]);

    stack_push(path_stack, next_square);
    stack_delete(&new_path_stack);

    return 1;
}

void init_maze( maze_t * maze,
                uint16_t squares_x,
                uint16_t squares_y)
{
    maze->length_x = squares_x;
    maze->length_y = squares_y;
    maze->squares = malloc(squares_x*squares_y*sizeof(square_t));
    clean_maze(maze);
}

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
                    uint16_t goal_color)
{
    // start of program
    maze->length_x = squares_x;
    maze->length_y = squares_y;

    // check if the size of the wall is larger then the minimun 
        // x side
    if (size_x > MIN_SIZE_IN_PIXEL)
    {
        maze->size_x = size_x;
    }else
    {
       maze->size_x = MIN_SIZE_IN_PIXEL;
    }
        // y side
    if (size_y > MIN_SIZE_IN_PIXEL)
    {
        maze->size_y = size_y;
    }else
    {
       maze->size_y = MIN_SIZE_IN_PIXEL;
    }
    
    // check if the actor is smaller than the square
        // x side
    if (actor_size_x <= (maze->size_x - 1))
    {
        maze->actor_size_x = actor_size_x;
    }else
    {
        maze->actor_size_x = (maze->size_x - 1);
    }
        // y side
    if (actor_size_y <= (maze->size_y - 1))
    {
        maze->actor_size_y = actor_size_y;
    }else
    {
        maze->actor_size_y = (maze->size_y - 1);
    }

    maze->start_pos.x = start_x;
    maze->start_pos.y = start_y;

    maze->end_pos.x = end_x;
    maze->end_pos.y = end_y;

    maze->BG_color    = BG_color;
    maze->wall_color  = wall_color;
    maze->actor_color = actor_color;
    maze->goal_color  = goal_color; 

    printf("setting maze\n");

    if (maze->squares == NULL)
    {
        printf("ERROR squares malloc\n");
        exit(EXIT_FAILURE);
    }
    
    clean_maze(maze);

    actor_stack_t path_stack;
    stack_create(&path_stack, squares_x*squares_y);

    actor_t actor;
    actor.x = start_x;
    actor.y = start_y;
    stack_push(&path_stack, actor);

    /**
     *  main part of the program
     */
    //variables
    actor_t last_actor;
    uint8_t timeout;
    uint16_t backup_timeout = 0;
    printf("start position: x:%d y:%d\n", stack_read_top(&path_stack).x,stack_read_top(&path_stack).y);
    //algorithm
    while (1)
    {
        last_actor = stack_read_top(&path_stack);

        // choose next square with error check
        if (goto_next_square(maze, &path_stack) == 0)
        {
            printf("!! done !!\n");
            break;
        }
        
        // timeout
        if ((last_actor.x == stack_read_top(&path_stack).x) &&
            (last_actor.y == stack_read_top(&path_stack).y))
        {
            timeout ++;
            if (timeout > TIMEOUT)
            {
                printf("------------------------\n------------------------\n------------------------\n");
                printf("!!timeout!!\n");
                break;
            }
        }else
        {
            timeout = 0;
        }

        if (backup_timeout >= 100000)
        {
            printf("------------------------\n------------------------\n------------------------\n");
            printf("failed\n");
            _debug_print(maze);
            exit(EXIT_FAILURE);
        }else
        {
            backup_timeout ++;
        }
    }

    //end of program
    //clean up
    stack_delete(&path_stack);
}

void write_maze_to_screenvalues(maze_t *maze,
                                uint16_t *screen_values,
                                uint16_t screen_length)
{
    uint16_t y;
    uint16_t x;
    for (y = 0; y < maze->length_y; y++)
    {
        for (x = 0; x < maze->length_x; x++)
        {
            uint32_t maze_index = COR_TO_INDEX(x,y,maze->length_x);
            uint8_t paths = (maze->squares[maze_index]) >> 4;
            if (GET_DIRECTION(paths, UP) != SET_DIRECTION(UP))
            {
                uint16_t start_x = x * maze->size_x;
                uint16_t y_pos = y * maze->size_y;
                uint16_t i;
                for (i = start_x; i < (start_x + maze->size_x); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(i, y_pos, screen_length);
                    screen_values[screen_index] = maze->wall_color;
                }
            }
            if (GET_DIRECTION(paths, RIGHT) != SET_DIRECTION(RIGHT))
            {
                uint16_t x_pos =( x * maze->size_x) + maze->size_x;
                uint16_t start_y = y * maze->size_y;
                uint16_t i;
                for (i = start_y; i < (start_y + maze->size_y); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(x_pos, i, screen_length);
                    screen_values[screen_index] = maze->wall_color;
                }
            }
            if (GET_DIRECTION(paths, DOWN) != SET_DIRECTION(DOWN))
            {
                uint16_t start_x = x * maze->size_x;
                uint16_t y_pos = (y * maze->size_y) + maze->size_y;
                uint16_t i;
                for (i = start_x; i < (start_x + maze->size_x); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(i, y_pos, screen_length);
                    screen_values[screen_index] = maze->wall_color;
                }
            }
            if (GET_DIRECTION(paths, LEFT) != SET_DIRECTION(LEFT))
            {
                uint16_t x_pos = x * maze->size_x;
                uint16_t start_y = y * maze->size_y;
                uint16_t i;
                for (i = start_y; i < (start_y + maze->size_y); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(x_pos, i, screen_length);
                    screen_values[screen_index] = maze->wall_color;
                }
            }
        }
    }

    // print the player in the start position
    uint16_t index;
    uint16_t actor_start_x = ((maze->start_pos.x * maze->size_x) + ((maze->size_x - maze->actor_size_x) / 2));
    uint16_t actor_start_y = ((maze->start_pos.y * maze->size_y) + ((maze->size_y - maze->actor_size_y) / 2));
    uint16_t actor_end_x   = actor_start_x + maze->actor_size_x - 1;
    uint16_t actor_end_y   = actor_start_y + maze->actor_size_y - 1;
    for ( x = actor_start_x; x < actor_end_x; x++)
    {
        for (y = actor_start_y; y < actor_end_y; y++)
        {
            index = COR_TO_INDEX(x,y,screen_length);
            screen_values[index] = maze->actor_color;
        }
    }
    // print the goal
}

void update_actor_screenvalues(maze_t *maze,
                               uint16_t *screen_values,
                               actor_t *old_actor,
                               actor_t *new_actor,
                               uint16_t screen_length,
                               int fbfd)
{
    // set the start and end values to center the actor
    uint16_t old_start_x = ((old_actor->x * maze->size_x) + ((maze->size_x - maze->actor_size_x) / 2));
    uint16_t old_start_y = ((old_actor->y * maze->size_y) + ((maze->size_y - maze->actor_size_y) / 2));
    uint16_t old_end_x   = old_start_x + maze->actor_size_x;
    uint16_t old_end_y   = old_start_y + maze->actor_size_y;

    uint16_t new_start_x = ((new_actor->x * maze->size_x) + ((maze->size_x - maze->actor_size_x) / 2));
    uint16_t new_start_y = ((new_actor->y * maze->size_y) + ((maze->size_y - maze->actor_size_y) / 2));
    uint16_t new_end_x   = new_start_x + maze->actor_size_x;
    uint16_t new_end_y   = new_start_y + maze->actor_size_y;


    uint16_t x;
    uint16_t y;
    uint16_t index;

    // clear the old actor
    for ( x = old_start_x; x < old_end_x; x++)
    {
        for (y = old_start_y; y < old_end_y; y++)
        {
            index = COR_TO_INDEX(x,y,screen_length);
            screen_values[index] = maze->BG_color;
        }
    }
    
    // set the new actor
    for ( x = new_start_x; x < new_end_x; x++)
    {
        for (y = new_start_y; y < new_end_y; y++)
        {
            index = COR_TO_INDEX(x,y,screen_length);
            screen_values[index] = maze->actor_color;
        }
    }

    //refresh the old actor area
    struct fb_copyarea old_actor_area;

    old_actor_area.dx     = old_start_x;
    old_actor_area.dy     = old_start_y;
    old_actor_area.height = maze->actor_size_y;
    old_actor_area.width  = maze->actor_size_x;

    screen_refresh(fbfd, &old_actor_area);

    // refresh the new actor area
    struct fb_copyarea new_actor_area;

    new_actor_area.dx     = new_start_x;
    new_actor_area.dy     = new_start_y;
    new_actor_area.height = maze->actor_size_y;
    new_actor_area.width  = maze->actor_size_x;

    screen_refresh(fbfd, &new_actor_area);


}

/**
 * @brief function for moving an actor
 * 
 * @return the function retuns an int to identify the success of the function.
 *         -1 for failed and 1 for found path and 2 for no path found   
 */
int move_actor(actor_t *actor,
               maze_t *maze, 
               direction_t direction,
               uint16_t *screen_values,
               uint16_t screen_length,
               int fbfd)
{
    // find the translated posistion
    actor_t translated_actor = translate_pos(*actor, direction);
    // check if a new path is found, -1 something went wrong, 1 found, 2 not found
    int found_path = -1;
    // get the indexes for the actor
    uint16_t actor_index     = COR_TO_INDEX(actor->x, actor->y, maze->length_x);

    //check if the transleted path is a valid path 
    if(GET_WALKED_DIRECTION(maze->squares[actor_index], direction))
    {
        //update actor on screen
        update_actor_screenvalues(maze,
                                  screen_values,
                                  actor,
                                  &translated_actor,
                                  screen_length,
                                  fbfd);
        //change actor location
        *actor = translated_actor;

        found_path = 1;
    }else
    {
        found_path = 2;
    }
    
    // return a check code to check the validity of the path
    return found_path;
}

/**
 * function for moving an actor, ignoring the walls
 * 
 * @name _move_actor_ignore_walls_
 * 
 * @brief ! For debugging, not use in actual game !.
 * 
 * @details Gives the player the possibility to move through the walls. 
 *       Simplifies the debugging process by allowing to finish maps faster
 */
void _move_actor_ignore_walls_(actor_t *actor,
                               maze_t *maze, 
                               direction_t direction,
                               uint16_t *screen_values,
                               uint16_t screen_length,
                               int fbfd)
{
    // find the translated posistion
    actor_t translated_actor = translate_pos(*actor, direction);

    //check if the transleted path is a valid path 
    if((translated_actor.x < maze->length_x) && (translated_actor.y < maze->length_y)
        && (translated_actor.x >= 0) && (translated_actor.y >= 0))
    {
        update_actor_screenvalues(maze,
                                  screen_values,
                                  actor,
                                  &translated_actor,
                                  screen_length,
                                  fbfd);

        *actor = translated_actor;
    }
}


void draw_goal( maze_t * maze,
                uint16_t *screen_values,
                uint16_t screen_length_x)
{
    uint16_t x;
    uint16_t y;
    uint16_t start_x = maze->end_pos.x * maze->size_x + 1;
    uint16_t start_y = maze->end_pos.y * maze->size_y + 1;
    uint16_t end_x = start_x + maze->size_x - 1;
    uint16_t end_y = start_y + maze->size_y - 1;
    uint16_t index;

    for ( x = start_x; x < end_x; x++)
    {
        for (y = start_y; y < end_y; y++)
        {
            index = COR_TO_INDEX(x,y,screen_length_x);
            screen_values[index] = maze->goal_color;
        }
    }   
}
