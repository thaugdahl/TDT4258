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
#include <signal.h>


//#include "game.h"
#include "display.h"
#include "henrik_test.h"

/*
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
*/
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
 * * TODO: create debugging printer(custom pretty system with options to disable)
 *
 * * TODO: create menu
 *
 * * TODO: create scoreboard
 * 		* TODO: create timer
 *
 */


/**
 * @brief Function for testing the movment for the actor or player
 * 
 * @param return returns the direction the actor should move
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
			printf("moving up\n");
			direction = UP;
			found_direction = 1;
			break;
		case 's':
			printf("moving down\n");
			direction = DOWN;
			found_direction = 1;
			break;
		case 'a':
			printf("moving left\n");
			direction = LEFT;
			found_direction = 1;
			break;
		case 'd':
			printf("moving right\n");
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


FILE* gamepad;
direction_t direction;
void gamepad_handler(){
	//int gpio_reg = map_input(fgetc(gamepad)); // <- ! This is cooked from internet!!
	int gpio_reg = fgetc(gamepad); 
	printf("Stop, don't touch me there, that is my no-no square\n");

	switch(gpio_reg)
	{
		case SW1:
			direction = LEFT;
			break;
		case SW2:
			direction = UP;
			break;
		case SW3:
			direction = RIGHT;
			break;
		case SW4:
			direction = DOWN;
			break;
		case SW5:
			direction = LEFT;
			break;
		case SW6:
			direction = UP;
			break;
		case SW7:
			direction = RIGHT;
			break;
		case SW8:
			direction = DOWN;
			break;
		default:
			break;
	}
}

int gamepad_init()
{
	int gpfd;
	printf("gamepad initalizing\n");

    gpfd = open("/dev/gamepad", O_RDONLY); // What the fuck is rb? (bad cooking)

	if (gpfd == -1) {
     	perror("Error: gamepad is kil");
     	exit(EXIT_FAILURE);
	}
	// !! The next X lines are copied from internet. I have no idea what they do. Educated guesses to what variables should be replaced to

	// Hard boiled code
	/*
	if (signal(SIGIO, &sigio_handler) == SIG_ERR) {
        printf("An error occurred while register a signal handler.\n");
        return EXIT_FAILURE;
    }
    if (fcntl(fileno(device), F_SETOWN, getpid()) == -1) {
        printf("Error setting pid as owner.\n");
        return EXIT_FAILURE;
    }
    long oflags = fcntl(fileno(device), F_GETFL);
    if (fcntl(fileno(device), F_SETFL, oflags | FASYNC) == -1) {
        printf("Error setting FASYNC flag.\n");
        return EXIT_FAILURE;
    }
	*/

	if (signal(SIGIO, &gamepad_handler)){
        printf("Signal handler registration goes \"brrrrrrr!\"\n");
        return EXIT_FAILURE;
    }
	if (fcntl(fileno(gamepad), F_SETOWN, getpid())){
        printf("Could not get pid, \"We have pid at home\"\n");
        return EXIT_FAILURE;
    }
	long oflags = fcntl(fileno(gamepad), F_GETFL);
	if (fcntl(fileno(gamepad), F_SETFL, oflags | FASYNC)) {
        printf("FASYNC flag says \"No\".\n");
        return EXIT_FAILURE;
    }
	printf("I'm giving her all she's got Cap'n!\n");

	return EXIT_SUCCESS;
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
	// TODO: check if it works on uClinux (TLDR: It do)
	srand(time(0));

	gamepad_init();

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
    generate_maze(6, 7, 20, 20, 10, 10, &maze, 1, 1, 3, 3, 0x0000, 0x00ff, 0xffff, 0x0380);
	actor_t player = maze.start_pos;
	printf("Actor color: (0x%x), wall color: (0x%x), BG color: (0x%x)\n", maze.actor_color, maze.wall_color, maze.BG_color);
	printf("\nMaze generated, let's print this shit\n");

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
		printf("player coords: x: %d ,y: %d\n", player.x, player.y);
		move_actor(	&player,
					&maze, 
					get_direction_from_terminal(),
					screen_values,
					SCREEN_SIZE_X,
					fbfd);

		// check if player has won
		if (player.x == maze.end_pos.x && player.y == maze.end_pos.y)
		{
			printf("victory!\n");
			game_loop = 0;
		}
		
	}

	// clean exit
	printf("Let's get the fuck out of this game\n");
	free(maze.squares);
	exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}


