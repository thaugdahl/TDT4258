#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

//#include "game.h"
#include "display.h"

//! Remember to comment everything!!!!!!!

/**
 * // TODO: create a display.c/.h (required)
 *  	TODO: create defines for screen size (required)
 * 		TODO: create function for writing to display (required)
 * 		TODO: create generic function for writing figures (required)
 *
 * TODO: port sound from ex2 (required)
 *
 * TODO: implement game controller (required)
 *
 * TODO: choose game (required)
 * 		?Game = maze game => simple and fun for the family ?
 *
 * * TODO: create debugging printer(custom pretty system with options to disable)
 *
 * * TODO: create menu
 *
 * * TODO: create scoreboard
 * 		* TODO: create timer for the maze
 *
 */

int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n\n");

	screen_init();
	printf("Hi game, I'm dad\n");

	//RED
	screen_fill(0b11111<<11);
	sleep(1);
	printf("Are ya winning, son?\n");

	//GREEN
	screen_fill(0b111111<<5);
	sleep(1);
	printf("Are YOU winning, dad?\n");

	//BLUE
	screen_fill(0b11111);
	sleep(1);
	printf("Son, I am dissapoint\n");


	rectangle_draw(150, 100, 70, 50, 0b11111<<11);

	printf("REEEEEEEEEEE\n");

	sleep(1);

	player_area_fill(0b111111<<5);

	sleep(2);

	screen_refresh();

	exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}


