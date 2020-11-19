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

#include "game.h"
#include "display.h"
#include "maze.h"

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


static FILE* gamepad;
maze_t maze;
actor_t player;
int fbfd;
volatile int game_loop = 1;
uint16_t *screen_values;
int screensize_bytes;
struct fb_copyarea full_screen_area;
int game_state;

void gamepad_handler(){
	direction_t direction;
	int found_valid_direction_from_gamepad_handler = 0;
	int gpio_reg =  (~fgetc(gamepad)) & 0xff; 
	switch(gpio_reg)
	{
		case SW1:
			direction = LEFT;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		case SW2:
			direction = UP;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		case SW3:
			direction = RIGHT;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		case SW4:
			direction = DOWN;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		case SW5:
			direction = LEFT;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		case SW6:
			direction = UP;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		case SW7:
			direction = RIGHT;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		case SW8:
			direction = DOWN;
			found_valid_direction_from_gamepad_handler = 1;
			break;
		default:
			break;
	}

	if(found_valid_direction_from_gamepad_handler)
	{
		// TODO: STOP CHEATING
		_move_actor_ignore_walls_(	&player,
				&maze, 
				direction,
				screen_values,
				SCREEN_SIZE_X,
				fbfd);
			
		printf("Player coords: x: %d , y: %d\n", player.x, player.y);
		// check if player has won
		if (player.x == maze.end_pos.x && player.y == maze.end_pos.y)
		{
			screen_fill(0xFFFF, screen_values, fbfd, &full_screen_area, screensize_bytes);
			screen_refresh(fbfd, &full_screen_area);
			printf("Victory!\n");
			game_state = GAME_PHINISH;
		}
		found_valid_direction_from_gamepad_handler = 0;
	}
}

int gamepad_init()
{
	printf("Gamepad initalizing\n");
    gamepad = fopen("/dev/gamepad", "rb");

	if (!gamepad) {
     	perror("Error: gamepad is kil");
     	exit(EXIT_FAILURE);
	}
	
	if (signal(SIGIO, &gamepad_handler)){
        printf("Signal handler registration goes \"brrrrrrr!\"\n");
        exit(EXIT_FAILURE);
    }
	int pid = getpid();
	
	printf("Mom, I want this pid %d\n",pid);
	if (fcntl(fileno(gamepad), F_SETOWN, pid) == -1){
        printf("Could not get pid, \"We have pid at home\"\npid at home: %x\n", pid);
		printf("fileno(gamepad)%d\n",fileno(gamepad));
        exit(EXIT_FAILURE);
    }
	long oflags = fcntl(fileno(gamepad), F_GETFL);
	if (fcntl(fileno(gamepad), F_SETFL, oflags | FASYNC)) {
        printf("FASYNC flag says \"No\".\n");
        exit(EXIT_FAILURE);
    }
	return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n\n");

	//set values constants
	full_screen_area.dx = 0;
	full_screen_area.dy = 0;
	full_screen_area.height = SCREEN_SIZE_Y;
	full_screen_area.width = SCREEN_SIZE_X;

	//set the seed value equal to the time
	srand(time(0));
	// screeninfo must be decleared on in main
	struct fb_var_screeninfo screeninfo;

	gamepad_init();
	screen_init(&screensize_bytes, 
				&fbfd,
				&screeninfo);
	// må bli mappet i main
	screen_values = (uint16_t*) mmap(NULL, screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	screen_fill(0x0000, screen_values, fbfd, &full_screen_area, screensize_bytes);
	screen_refresh(fbfd, &full_screen_area);
	init_maze(&maze,8,7);
	game_state = GAME_STARTUP;
	//game loop
	while (game_loop == 1)
	{
		switch (game_state)
		{
		case GAME_STARTUP:
			printf("Game is initalizing\n");
			generate_maze(8, 7, 20, 20, 10, 10, &maze, 1, 1, 1, 2, 0x0000, 0x00ff, 0xffff, 0x0380);
			player = maze.start_pos;
			screen_fill(maze.BG_color, screen_values, fbfd, &full_screen_area, screensize_bytes);
			screen_refresh(fbfd, &full_screen_area);
			write_maze_to_screenvalues(&maze, screen_values, SCREEN_SIZE_X);
			draw_goal(&maze, screen_values, SCREEN_SIZE_X);
			screen_refresh(fbfd, &full_screen_area);
			printf("Game initalized\n");
			game_state = GAME_RUNNING;
			break;
			
		case GAME_RUNNING:
			__asm__ ("wfi");
			break;

		case GAME_PHINISH:
			game_state = GAME_STARTUP;
			//free(maze.squares);
			break;

		default:
			printf("ERROR: no valid state, returning to startup\n");
			game_state = GAME_STARTUP; 
			break;
		}
	}

	// clean exit
	free(maze.squares);
	fclose(gamepad);
	printf("Exiting game\n");

	exit(EXIT_SUCCESS);
}


