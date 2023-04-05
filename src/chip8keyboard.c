#include "chip8keyboard.h"
#include <assert.h>
#include <conio.h>


static void chip8_keyboard_ensure_inbounds(int key){
    assert(key >= 0 && key < CHIP8_TOTAL_KEYS);
}

int chip8_kryboard_map(const char* map, char key){
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++)
    {
        if (map[i] == key)
        {
            return i;
        }
    }
    return -1;
}

void chip8_keyboard_down(struct chip8keyboard* keyboard, int key){
    chip8_keyboard_ensure_inbounds(key);
    keyboard->keyboard[key] = true;
}
void chip8_keyboard_up(struct chip8keyboard* keyboard, int key){
    chip8_keyboard_ensure_inbounds(key);
    keyboard->keyboard[key] = false;
}
bool chip8_keyboard_is_down(struct chip8keyboard* keyboard, int key){
    chip8_keyboard_ensure_inbounds(key);
    return keyboard->keyboard[key];
}
