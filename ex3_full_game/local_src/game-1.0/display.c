#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

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
* Values written to screen_values[] are printed on screen TODO: Verify that this comment is true
*/
void screen_refresh()
{
	printf("refreshing screen\n");
	ioctl(fbfd, 0x4680, &rect);
	printf("refreshed screen\n");
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
	
	//comand driver to update display
	//screen_refresh();
}

void rectangle_draw(int x_pos, int y_pos, int width, int height, int color) 
{
	rect.dx = x_pos;
	rect.dy = y_pos;
	rect.height = height;
	rect.width = width;
	rect.color = color;
	screen_refresh();
}

void rectangle_up()
{
	rect.dx += 1;
	refresh_screen();
}

void rectangle_down()
{
	rect.dx -= 1;
	refresh_screen();
}