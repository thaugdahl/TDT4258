#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

//! Remember to comment everything!!!!!!!

/**
 * TODO: create a display.c/.h (required)
 *  
 * TODO: create defines for screen size (required)
 * 
 * TODO: port sound from ex2 (required)
 * 
 * TODO: implement game controller (required)
 * 
 * TODO: choose game (required)
 * 
 * * TODO: create debugging printer(custom pretty system with options to disable)
 * 
 * * TODO: create menu
 * 
 * * TODO: create scoreboard
 * 
 */


int screensize_bytes;
int fbfd;
uint16_t *screen_values;
struct fb_copyarea rect;
struct fb_var_screeninfo screen_info;


void refresh_screen(void);
void fillscreen(uint16_t value);


/**
* @brief refreshes screen
* 
* Values written to screen_values[] are printed on screen
*/
void refresh_screen(){
	
	printf("refreshing screen\n");
	ioctl(fbfd, 0x4680, &rect);
	printf("refreshed screen\n");
	
}

/**
* @brief fill screen
* 
* @param value holds the color that is used to fill the screen (bit 15-11=red, bit 10-5=blue, bit 4-0=green)
* Fills 5000 pixels with the color that is given in the parameter value TODO: 5000 pixels? How to fix?
*/
void fill_screen(uint16_t value){
	uint32_t i = 0;
	
	for(i=0; i<5000; i++){
		screen_values[i]=value;
	}
	
	//comand driver to update display
	refresh_screen();
}


int main(int argc, char *argv[])
{
	printf("Hello World, I'm game!\n\n");

	fbfd = open("/dev/fb0", O_RDWR);
	printf("fbfd value: %d\n", fbfd);
	if (fbfd == -1) {
     	perror("Error: cannot open framebuffer device");
     	exit(1);
	}

	printf("Hi game, I'm dad\n");

	if(ioctl(fbfd, FBIOGET_VSCREENINFO, &screen_info) == -1){
		printf("Error: failed to get screen info\n");
		return -1;
	} 

	printf("Are ya winning, son?\n");

	screensize_bytes = (320 * 240) * screen_info.bits_per_pixel/8;
	screen_values = (uint16_t*) mmap(NULL, screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	
	// setup which part of the frame buffer that is to be refreshed
	// for performance reasons , use as small rectangle as possible
	//struct fb_copyarea rect;
	//struct fb_fillrect rect;
	//printf("%u",screen_values[0]);
	
	printf("REEEEEEEEEEEEEd\n");
	rect.dx = 10;
	rect.dy = 10;
	rect.height = 169;
	rect.width = 169;
	
	
	fill_screen(0xFF);

	//close(fbfd);

	exit(EXIT_SUCCESS);
}
