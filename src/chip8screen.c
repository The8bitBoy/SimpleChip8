#include "chip8screen.h"
#include <assert.h>
#include <stdio.h>
#include <memory.h>

static void chip8_pixels_is_inbound(int x, int y){
    assert(x >= 0 && y >= 0);
    assert(x < CHIP8_SCREEN_WIDTH && y < CHIP8_SCREEN_HEIGHT);
}

void chip8screen_set(struct chip8screen* screen, int x, int y){
    chip8_pixels_is_inbound(x, y);
    screen->pixels[y][x] = true;
}

void chip8screen_unset(struct chip8screen* screen, int x, int y){
    chip8_pixels_is_inbound(x, y);
    screen->pixels[y][x] = false;
}

bool chip8screen_is_set(struct chip8screen* screen, int x, int y){
    chip8_pixels_is_inbound(x, y);
    return screen->pixels[y][x];
}

bool chip8screen_draw_sprite(struct chip8screen* screen, int x, int y, const char* sprite, int num){
    bool pixel_collison = false;

    for (int line = 0; line < num; line++)
    {
        char line_byte = sprite[line];
        for (int offset = 0; offset < 8; offset++)
        {
            if ((line_byte & (0b10000000 >> offset)) == 0)
                continue;
            int pixel_x = x + offset;
            int pixel_y = y + line;

            pixel_x = fix_x_over_screen(pixel_x);
            pixel_y = fix_y_over_screen(pixel_y);

            bool perviuse_pixel = chip8screen_is_set(screen, pixel_x, pixel_y);
            bool final_pixel = perviuse_pixel ^ 1; // XOR with 1
            if (final_pixel)
            {
                chip8screen_set(screen, pixel_x, pixel_y);
            } else {
                chip8screen_unset(screen, pixel_x, pixel_y);
                pixel_collison = true;
            }
        }
    }

    return pixel_collison;
}

int fix_x_over_screen(int x){
    return (x % CHIP8_SCREEN_WIDTH);
}

int fix_y_over_screen(int y){
    return (y % CHIP8_SCREEN_HEIGHT);
}

void chip8screen_clear(struct chip8screen* screen){
    memset(screen->pixels, 0, sizeof(screen->pixels));
}