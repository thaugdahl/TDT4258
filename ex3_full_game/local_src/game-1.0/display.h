#ifndef _DISPLAY_H
#define _DISPLAY_H

#define SCREEN_SIZE_X 320
#define SCREEN_SIZE_Y 240
#define SCREEN_LENGTH (SCREEN_SIZE_X * SCREEN_SIZE_Y)

void screen_init();
void screen_refresh(void);
void screen_fill(uint16_t value);

void player_area_refresh();
void player_area_fill(uint16_t value);

//void update_element(&figure, struct vec_3 rect_new_location);

void rectangle_draw(int x_pos, int y_pos, int width, int height, uint16_t color);



#endif