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

/**
* @brief initilize the screen
*
*
*/

void screen_init(int *screensize_bytes, 
				 int *fbfd,
				 struct fb_var_screeninfo *screeninfo)
{
	printf("screen initalizing\n");
	
    *fbfd = open("/dev/fb0", O_RDWR);
	printf("fbfd value: %d\n", *fbfd);

	if (*fbfd == -1) {
     	perror("Error: cannot open framebuffer device");
     	exit(EXIT_FAILURE);
	}

	if(ioctl(*fbfd, FBIOGET_VSCREENINFO, screeninfo) == -1){
		perror("Error: failed to get screen info\n");
		exit(EXIT_FAILURE);
	}

   	*screensize_bytes = SCREEN_LENGTH * screeninfo->bits_per_pixel/8;
	//screen_values = (uint16_t*) mmap(NULL, *screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, *fbfd, 0);
    printf("screen initialized successfully\n");
}

/**
* @brief refreshes screen
*
* Values written to screen_values[] are printed on screen
*/

void screen_refresh(int fbfd, struct fb_copyarea *copyarea)
{
	ioctl(fbfd, 0x4680, copyarea);
}

void player_area_fill(uint16_t value, 
					  uint16_t *screen_values, 
					  int fbfd, 
					  struct fb_copyarea *player_area,
					  int screensize_bytes)
{
	uint32_t i = 0;
	for(i=0; i<screensize_bytes; i++){
		screen_values[i]=value;
	}

	screen_refresh(fbfd, player_area);
}

/*
uint16_t rgb_to_16_bit_color(int red, int green, int blue) {
	uint16_t color = 0;
	color |= blue;
	color |= green << 5;
	color |= red << 11;
	return color;
}
*/


/**
* @brief fill screen
*
* @param value holds the color that is used to fill the screen (bit 15-11=red, bit 10-5=green, bit 4-0=blue)
* Fills all pixels with the color that is given in the parameter value
*/

void screen_fill(	uint16_t value, 
				 	uint16_t *screen_values, 
				 	int fbfd, 
					struct fb_copyarea *copyarea,
					int screensize_bytes)
{
	uint32_t i = 0;

	for(i=0; i<screensize_bytes; i++){
		screen_values[i]=value;
	}

	//command driver to update display
	screen_refresh(fbfd, copyarea);
}

void rectangle_draw(int x_pos, 
					int y_pos, 
					int width, 
					int height, 
					uint16_t color, 
					uint16_t *screen_values,
					int fbfd, 
					struct fb_copyarea *copyarea)
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

	screen_refresh(fbfd, copyarea);
}

