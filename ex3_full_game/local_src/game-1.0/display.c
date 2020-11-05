#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <memory.h>

#include "display.h"

struct fb_var_screeninfo screen_info;
int screensize_bytes;
int fbfd;

uint16_t *screen_values;
struct fb_copyarea rect;


/**
* @brief initilize the screen
*
*
*/

void screen_init()
{
	printf("screen initalizing\n");

	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;

	player_area.dx 		= 0;
	player_area.dy 		= 220;
	player_area.width 	= 320;
	player_area.height 	= 20;

    fbfd = open("/dev/fb0", O_RDWR);
	printf("fbfd value: %d\n", fbfd);

	if (fbfd == -1) {
     	perror("Error: cannot open framebuffer device");
     	exit(EXIT_FAILURE);
	}

	if(ioctl(fbfd, FBIOGET_VSCREENINFO, &screen_info) == -1){
		perror("Error: failed to get screen info\n");
		exit(EXIT_FAILURE);
	}

   	screensize_bytes = SCREEN_LENGTH * screen_info.bits_per_pixel/8;
	screen_values = (uint16_t*) mmap(NULL, screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    printf("screen initialized successfully\n");
}

/**
* @brief refreshes screen
*
* Values written to screen_values[] are printed on screen
*/

void screen_refresh()
{
	ioctl(fbfd, 0x4680, &rect);
}

void player_area_refresh()
{
	ioctl(fbfd, 0x4680, &player_area);
}

void player_area_fill(uint16_t value)
{
	uint32_t i = 0;
	for(i=0; i<screensize_bytes; i++){
		screen_values[i]=value;
	}

	player_area_refresh();
}

/**
* @brief fill screen
*
* @param value holds the color that is used to fill the screen (bit 15-11=red, bit 10-5=green, bit 4-0=blue)
* Fills all pixels with the color that is given in the parameter value
*/

void screen_fill(uint16_t value)
{
	uint32_t i = 0;

	for(i=0; i<screensize_bytes; i++){
		screen_values[i]=value;
	}

	//command driver to update display
	screen_refresh();
}

void rectangle_draw(int x_pos, int y_pos, int width, int height, uint16_t color)
{
	int y;
	int x;
	if (x_pos+width > SCREEN_SIZE_X || y_pos+height > SCREEN_SIZE_Y)
	{
		perror("Error: Drawing rectangle out of screen bounds");
		exit(EXIT_FAILURE);
	}

	for (y = y_pos; y < height+y_pos; y++)
	{
		for(x = x_pos; x < width+x_pos; x++)
		{
			int pos = x+y*SCREEN_SIZE_X;
			screen_values[pos] = color;
		}
	}

	screen_refresh();
}

/// !!!!!!!!!!!!!!!!!!!!! for testing

	rect.dx = 0;
	rect.dy = 0;
	rect.width = 320;
	rect.height = 240;

	player_area.dx 		= &rect.dx;
	player_area.dy 		= &rect.dy;
	player_area.width 	= 320;
	player_area.height 	= 20;



struct player = {
	x
	y
	x_next
	y_next
}
	struct fb_copyarea player_area;
player_area.dx = &player.x
player.x_next = x+dx

if player.x_next = vegg
player.x_next = player.x
break;
else
figure_draw(&player)
	screen_values -= player.x
	screen_values += player.x_next

#define vei = 0
#define vegg = 1

0001101010010
000111001010

color = 0xff*(~pixel)
