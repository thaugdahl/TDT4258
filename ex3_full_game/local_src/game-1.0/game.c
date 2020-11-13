//std libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

//#include "game.h"
#include "display.h"
#include "henrik_test.h"

/**
void 😮()
{
	printf("REEEEEEEE\n")
	//printf("😮");
}
*/

//! Remember to comment everything!!!!!!!

/**
 * 
 * final things todo:
 * 		TODO: implement game controller (required)
 * 		TODO: create generic function for writing figures (required)
 * 		TODO: write in  overleaf! (required)
 * 		TODO: create goal (required)
 * 	*	TODO: create simple menu to start game (optional)
 * 		TODO: rask clean up, legge til funksjons forklaringer
 * 
 * // TODO: create a display.c/.h (required)
 *  	// TODO: create defines for screen size (required)
 * 		// TODO: create function for writing to display (required)
 *
 * 
 *
 * // TODO: choose game (required)
 * 		?Game = maze game => simple and fun for the family ?
 *
 * * TODO: port sound from ex2
 *
 * * TODO: create debugging printer(custom pretty system with options to disable)
 *
 * * TODO: create menu
 *
 * * TODO: create scoreboard
 * 		* TODO: create timer
 *
 */

direction_t get_direction_from_terminal(void)
{
	char direction_from_scanf;
	direction_t direction;
	uint8_t found_direction = 0;
	while (found_direction == 0)
	{
		// collect direction
		printf("select direction(w,a,s,d): \n");
		scanf("%c", &direction_from_scanf);

		// check if it is a valid direction
		switch (direction_from_scanf)
		{
		case 'w':
			printf("moving up");
			direction = UP;
			found_direction = 1;
			break;
		case 's':
			printf("moving down");
			direction = DOWN;
			found_direction = 1;
			break;
		case 'a':
			printf("moving left");
			direction = LEFT;
			found_direction = 1;
			break;
		case 'd':
			printf("moving right");
			direction = RIGHT;
			found_direction = 1;
			break;	
		default:
			printf("select valid dirction!\n");
			found_direction = 0;
			break;
		}
	}
	return direction;
}

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n\n");

	//init varables
	int screensize_bytes;
	//uint16_t screen_a[];
	// uint16_t *screen_values = &screen_a;
	struct fb_var_screeninfo screen_info;
	struct fb_copyarea full_screen_area;
	int fbfd;
	maze_t maze;
	int game_loop = 1;
	//set values constants
	full_screen_area.dx = 0;
	full_screen_area.dy = 0;
	full_screen_area.height = SCREEN_SIZE_Y;
	full_screen_area.width = SCREEN_SIZE_X;

	//set the seed value equal to the time
	// TODO: check if it works on uClinux
	srand(time(0));

	// TODO: Initialisere knapper. Kan vi bruke mmap for å ha tilgang til knappene?
	
	//init screeen
	screen_init(&screensize_bytes, 
				&fbfd,
				&screen_info);
	// må bli mappet i main
	uint16_t *screen_values = (uint16_t*) mmap(NULL, screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	printf("1: screen values: %p\n", screen_values);

	printf("Hi game, I'm dad\n");

	screen_fill(0x0000, screen_values, fbfd, &full_screen_area, screensize_bytes);

	printf("2: screen values: %p\n",screen_values);

	sleep(1);
	screen_refresh(fbfd, &full_screen_area);
	sleep(2);
	printf("hey dad im a maze\n");
	sleep(5);

	// Currently, maximum size is: 6x7 tiles
    generate_maze(6, 7, 10, 10, 2, 2, &maze, 1, 1, 3, 3, 0x0000, 0xffff, 0xffff);
	actor_t player = maze.start_pos;
	printf("\nMaze generated, let's print this shit\n");

	//maze_t test_maze;
	//test_maze.length_x = 10;
    //test_maze.length_y = 10;
    //test_maze.size_x = 20;
    //test_maze.size_y = 20;
    //test_maze.start_pos.x = 1;
    //test_maze.start_pos.y = 1;
	//printf("time to map the test\n");
    //test_maze.squares = malloc(10*10*sizeof(square_t));
	//printf("time to init the test\n");
	//init_maze(&test_maze);


	screen_fill(maze.BG_color, screen_values, fbfd, &full_screen_area, screensize_bytes);

	printf("time to write the test to the screen\n");
	write_maze_to_screenvalues(&maze, screen_values, SCREEN_SIZE_X);

	printf("This shit printed\n");

	screen_refresh(fbfd, &full_screen_area);

	sleep(1);
	
	//game loop
	// TODO: create game mechanics
	while (game_loop == 1)
	{
		sleep(1);
		move_actor(&player,
								  &maze, 
								  get_direction_from_terminal(),
								  screen_values,
								  SCREEN_SIZE_X,
								  fbfd);
	}

	// clean exit
	printf("Let's get the fuck out of this game\n");
	free(maze.squares);
	exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}


