#include "chip8stack.h"
#include "chip8.h"
#include <assert.h>


static void chip8_stack_in_bound(struct chip8* chip){
    assert(chip->registers.SP < sizeof(chip->stack.stack));
}

void chip8_stack_push(struct chip8* chip, unsigned short val){
    chip->registers.SP += 1;
    chip8_stack_in_bound(chip);
    chip->stack.stack[chip->registers.SP] = val;
}

unsigned short chip8_stack_pop(struct chip8* chip){
    chip8_stack_in_bound(chip);
    unsigned short result = chip->stack.stack[chip->registers.SP];
    chip->registers.SP -= 1;
    return result;
}
