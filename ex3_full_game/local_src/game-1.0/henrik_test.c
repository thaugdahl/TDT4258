#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "henrik_test.h"

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
    }
}

//void stack_remove(actor_stack_t *stack, actor_t *actor, uint16_t element)
//{
//    if ((element < stack->head) && (stack->head >= 0))
//    {
//        actor_t *temp = malloc((element+1)*sizeof(actor_t));
//
//        for (uint16_t i = 0; i < element; i++)
//        {
//            temp[i] = stack_pop(stack);
//        }
//
//        *actor = stack_pop(stack);
//
//        for (uint16_t i = 0; i < element; i++)
//        {
//            stack_push(stack, temp[i]);
//        }
//
//        free(temp);
//    }
//}

void stack_clear(actor_stack_t *stack, uint16_t top)
{
    stack->head = -1;
    stack->MAX = top;
    stack->stack = malloc(top*sizeof(actor_t));
}

void stack_create(actor_stack_t *stack, uint16_t top)
{
    stack_clear(stack, top);
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

actor_t translate_pos(actor_t actor, uint8_t direction)
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
    int dx = last_actor.x - next_actor.x;
    int dy = last_actor.y - next_actor.y;

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

void init_maze(maze_t *maze)
{
    uint8_t index;
    for (pos_t y = 0; y < maze->length_y; y++)
    {
        for (pos_t x = 0; x < maze->length_x; x++)
        {
            index = COR_TO_INDEX(x, y, maze->length_x);
            maze->squares[index].paths = 0b00001111;
            maze->squares[index].visited = (uint8_t)0;

            if (x == 0)
            {
                maze->squares[index].paths &= ~SET_VALID_DIRECTION(LEFT);
            }
            else if (x == (maze->length_x - 1))
            {
                maze->squares[index].paths &= ~SET_VALID_DIRECTION(RIGHT);
            }

            if (y == 0)
            {
                maze->squares[index].paths &= ~SET_VALID_DIRECTION(UP);
            }
            else if (y == (maze->length_y - 1))
            {
                maze->squares[index].paths &= ~SET_VALID_DIRECTION(DOWN);
            }
        }
    }
    maze->squares[COR_TO_INDEX(maze->start_pos.x, maze->start_pos.y, maze->length_x)].visited = 1;
}

void _debug_print(maze_t *maze)
{
    printf("------------------------\n------------------------\n------------------------\n");
    uint32_t controll = 0;
    for (int y = 0; y < maze->length_y; y++)
    {
        for (int x = 0; x < maze->length_x; x++)
        {
            uint8_t visited = maze->squares[COR_TO_INDEX(x,y,maze->length_x)].visited;
            printf("x:%d y:%d v:%d i:%d \n",
                   x,y,visited&1,COR_TO_INDEX(x,y,maze->length_x));
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
    uint8_t index = COR_TO_INDEX(stack_read_top(path_stack).x,
                                 stack_read_top(path_stack).y,
                                 maze->length_x);
    uint8_t test_index;
    actor_t next_square;
    actor_t test_pos;

    // test up
    if(GET_VALID_DIRECTION(maze->squares[index].paths, UP))
    {
        test_pos = translate_pos(stack_read_top(path_stack), UP);
        test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
        if (((maze->squares[test_index].visited & 1) == 0) &&
            (test_pos.x < maze->length_x) &&
            (test_pos.y < maze->length_y))
        {
            stack_push(&new_path_stack, test_pos);
            printf("found possible path: x:%d y:%d\n",test_pos.x, test_pos.y);
        }else
        {
            maze->squares[index].paths &= ~SET_VALID_DIRECTION(UP);
            maze->squares[test_index].paths &= ~SET_VALID_DIRECTION(DOWN);
        }
    }

    // test right
    if(GET_VALID_DIRECTION(maze->squares[index].paths, RIGHT))
    {
        test_pos = translate_pos(stack_read_top(path_stack), RIGHT);
        test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
        if (((maze->squares[test_index].visited & 1) == 0) &&
            (test_pos.x < maze->length_x) &&
            (test_pos.y < maze->length_y))
        {
            stack_push(&new_path_stack, test_pos);
            printf("found possible path: x:%d y:%d\n",test_pos.x, test_pos.y);

        }else
        {
            maze->squares[index].paths &= ~SET_VALID_DIRECTION(RIGHT);
            maze->squares[test_index].paths &= ~SET_VALID_DIRECTION(LEFT);
        }
    }

    // test down
    if(GET_VALID_DIRECTION(maze->squares[index].paths, DOWN))
    {
        test_pos = translate_pos(stack_read_top(path_stack), DOWN);
        test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
        if (((maze->squares[test_index].visited & 1) == 0) &&
            (test_pos.x < maze->length_x) &&
            (test_pos.y < maze->length_y))
        {
            stack_push(&new_path_stack, test_pos);
            printf("found possible path: x:%d y:%d\n",test_pos.x, test_pos.y);

        }else
        {
            maze->squares[index].paths &= ~SET_VALID_DIRECTION(DOWN);
            maze->squares[test_index].paths &= ~SET_VALID_DIRECTION(UP);
        }
    }

    // test left
    if(GET_VALID_DIRECTION(maze->squares[index].paths, LEFT))
    {
        test_pos = translate_pos(stack_read_top(path_stack), LEFT);
        test_index = COR_TO_INDEX(test_pos.x, test_pos.y, maze->length_x);
        if (((maze->squares[test_index].visited & 1) == 0) &&
            (test_pos.x < maze->length_x) &&
            (test_pos.y < maze->length_y))
        {
            stack_push(&new_path_stack, test_pos);
            printf("found possible path: x:%d y:%d\n",test_pos.x, test_pos.y);
        }else
        {
            maze->squares[index].paths &= ~SET_VALID_DIRECTION(LEFT);
            maze->squares[test_index].paths &= ~SET_VALID_DIRECTION(RIGHT);
        }
    }

    //check len of stack, if 0 then go back
    if (stack_length(&new_path_stack) > 0)
    {
        if (stack_length(&new_path_stack) == 1)
        {
            next_square = stack_pop(&new_path_stack);
        }else
        {
            int new_path_index;
            new_path_index = rand() % (stack_length(&new_path_stack));
            printf("new path index:%d\n",new_path_index);
            for (int i = 0; i < new_path_index; i++)
            {
                stack_pop(&new_path_stack);
            }
            next_square = stack_pop(&new_path_stack);
            //stack_remove(&new_path_stack, &next_square, new_path_index);
        }
    }else
    {
        if (stack_length(path_stack) > 1)
        {
            stack_pop(path_stack);
            next_square = stack_pop(path_stack);
        }else
        {
            return 0;
        }
    }

    printf("next: x:%d y:%d\n", next_square.x,next_square.y);
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

    maze->squares[last_index].paths |= SET_WALKED_DIRECTION(direction_to_new);
    maze->squares[next_index].paths |= SET_WALKED_DIRECTION(direction_from_new);

    maze->squares[next_index].visited = (uint8_t)1;

    stack_push(path_stack, next_square);
    stack_delete(&new_path_stack);

    return 1;
}




void generate_maze( pos_t    squares_x,
                    pos_t    squares_y,
                    uint16_t screen_x,
                    uint16_t screen_y,
                    maze_t   *maze,
                    pos_t    start_x,
                    pos_t    start_y,
                    pos_t    end_x,
                    pos_t    end_y)
{
    // start of program
    maze->length_x = squares_x;
    maze->length_y = squares_y;

    maze->size_x = (uint16_t)(screen_x/squares_x);
    maze->size_y = (uint16_t)(screen_y/squares_y);

    maze->start_pos.x = start_x;
    maze->start_pos.y = start_y;

    maze->squares = malloc(squares_x*squares_y*sizeof(square_t));

    init_maze(maze);

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
    actor_t next_actor;
    actor_t current_actor;
    uint8_t timeout;
    uint16_t backup_timeout = 0;
    printf("start: x:%d y:%d\n", stack_read_top(&path_stack).x,stack_read_top(&path_stack).y);
    //algorithm
    while (1)
    {
        last_actor = stack_read_top(&path_stack);

        // choose next square with error check
        if (goto_next_square(maze, &path_stack) == 0)
        {
            printf("\n\n!! done !!\n\n");
            break;
        }

        printf("x:%d y:%d len:%d\n\n", stack_read_top(&path_stack).x,
               stack_read_top(&path_stack).y, path_stack.head);

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

        if (backup_timeout >= 50)
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

void write_maze_to_screenvalues(maze_t *maze, uint16_t *screen_values,
                                uint16_t wall_color, uint16_t screen_length,
                                uint16_t screen_hight)
{
    for (uint16_t y = 0; y < maze->length_y; y++)
    {
        for (uint16_t x = 0; x < maze->length_x; x++)
        {
            uint32_t maze_index = COR_TO_INDEX(x,y,maze->length_x);
            uint8_t paths = (maze->squares[maze_index].paths
                             & WALKED_PATH_MASK) >> 4;

            if (GET_DIRECTION(paths, UP) == SET_DIRECTION(UP))
            {
                uint16_t start_x = x * maze->size_x;
                uint16_t y_pos = y * maze->size_y;
                for (uint16_t i = start_x; i < (start_x + maze->size_x); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(i, y_pos, screen_length);
                    screen_values[screen_index] = wall_color;
                }
            }
            if (GET_DIRECTION(paths, RIGHT) == SET_DIRECTION(RIGHT))
            {
                uint16_t x_pos = x * maze->size_x + maze->size_x;
                uint16_t start_y = y * maze->size_y;
                for (uint16_t i = start_y; i < (start_y + maze->size_y); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(x_pos, i, screen_length);
                    screen_values[screen_index] = wall_color;
                }
            }
            if (GET_DIRECTION(paths, DOWN) == SET_DIRECTION(DOWN))
            {
                uint16_t start_x = x * maze->size_x;
                uint16_t y_pos = y * maze->size_y + maze->size_y;
                for (uint16_t i = start_x; i < (start_x + maze->size_x); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(i, y_pos, screen_length);
                    screen_values[screen_index] = wall_color;
                }
            }
            if (GET_DIRECTION(paths, LEFT) == SET_DIRECTION(LEFT))
            {
                uint16_t x_pos = x * maze->size_x;
                uint16_t start_y = y * maze->size_y;
                for (uint16_t i = start_y; i < (start_y + maze->size_y); i++)
                {
                    uint16_t screen_index = COR_TO_INDEX(x_pos, i, screen_length);
                    screen_values[screen_index] = wall_color;
                }
            }
        }
    }
}


void main()
{
    srand(time(0));
    maze_t maze;
    generate_maze(5, 5, 100, 100, &maze, 1,1,3,3);
    _debug_print(&maze);
    /**
    //test
    actor_stack_t test;
    actor_t test1;
    test1.x = 1;
    test1.y = 1;
    actor_t test2;
    test2.x = 2;
    test2.y = 2;
    stack_create(&test,5);
    stack_push(&test, test1);
    stack_push(&test, test2);
    actor_t read1 = stack_pop(&test);
    printf("x:%d, y:%d", read1.x, read1.y);
    */
    exit(EXIT_SUCCESS);
}