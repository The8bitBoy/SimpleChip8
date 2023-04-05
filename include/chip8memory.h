#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H
#include "config.h"

struct chip8memory
{
    char memory[CHIP8_MEMORY_SIZE];
};

void chip8memory_set(struct chip8memory* memory, int index, char val);
unsigned char chip8memory_get(struct chip8memory* memory, int index);
unsigned short chip8memory_get_short(struct chip8memory* memory, int index);

#endif