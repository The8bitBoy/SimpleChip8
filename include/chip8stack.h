#ifndef CHIP8STACK_H
#define CHIP8STACK_H
#include "config.h"

struct chip8;
struct chip8stack
{
    unsigned short stack[CHIP8_TOTAOL_STACK_DEPTH];
};

void chip8_stack_push(struct chip8* chip, unsigned short val);
unsigned short chip8_stack_pop(struct chip8* chip);

#endif