#ifndef _DISPLAY_H
#define _DISPLAY_H

#define SCREEN_SIZE_X 320
#define SCREEN_SIZE_Y 240

// The total number of pixels in the screen
#define SCREEN_LENGTH (SCREEN_SIZE_X * SCREEN_SIZE_Y)

// convert rgb to an 16 bit unsigned integer
// todo: test the function
#define rgb_to_16_bit_color(r,g,b) ((b & 0b11111) | (g & 0b111111) << 5 | (r & 0b11111) << 11)


void screen_init(int *screensize_bytes, 
				 int *fbfd,
				 struct fb_var_screeninfo *screeninfo);
                 
void screen_refresh(int fbfd, struct fb_copyarea *copyarea);

void screen_fill(	uint16_t value, 
				 	uint16_t *screen_values, 
				 	int fbfd, 
					struct fb_copyarea *copyarea,
					int screensize_bytes);

void player_area_fill(uint16_t value, 
					  uint16_t *screen_values, 
					  int fbfd, 
					  struct fb_copyarea *player_area,
                      int screensize_bytes);

//void update_element(&figure, struct vec_3 rect_new_location);

void rectangle_draw(int x_pos, 
                    int y_pos, 
                    int width,
                    int height, 
                    uint16_t color, 
                    uint16_t *screen_values, 
                    int fbfd, 
                    struct fb_copyarea *copyarea);

#endif