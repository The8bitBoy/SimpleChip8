#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include <stdbool.h>
#include "config.h"

struct chip8screen
{
    bool pixels[CHIP8_SCREEN_HEIGHT][CHIP8_SCREEN_WIDTH];
};

void chip8screen_set(struct chip8screen* screen, int x, int y);
bool chip8screen_is_set(struct chip8screen* screen, int x, int y);
bool chip8screen_draw_sprite(struct chip8screen* screen, int x, int y, const char* sprite, int num);
void chip8screen_unset(struct chip8screen* screen, int x, int y);
int fix_y_over_screen(int y);
int fix_x_over_screen(int x);
void chip8screen_clear(struct chip8screen* screen);


#endif