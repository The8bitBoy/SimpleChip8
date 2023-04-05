#ifndef CHIP8KEYBOARD_H
#define CHIP8KEYBOARD_H

#include <stdbool.h>
#include "config.h"

struct chip8keyboard
{
    bool keyboard[CHIP8_TOTAL_KEYS];
};

typedef enum {
    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
    KEY_a = 'a',
    KEY_b = 'b',
    KEY_c = 'c',
    KEY_d = 'd',
    KEY_e = 'e',
    KEY_f = 'f'
} chip8_keycode;

int chip8_kryboard_map(const char* map, char key);
void chip8_keyboard_down(struct chip8keyboard* keyboard, int key);
void chip8_keyboard_up(struct chip8keyboard* keyboard, int key);
bool chip8_keyboard_is_down(struct chip8keyboard* keyboard, int key);

#endif