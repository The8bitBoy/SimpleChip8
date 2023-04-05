#ifndef CHIP8_H
#define CHIP8_H

#include <stddef.h>
#include "config.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8screen.h"

struct chip8{
    
    struct chip8memory memory;
    struct chip8stack stack;
    struct chip8_registers registers;
    struct chip8keyboard keyboard;
    struct chip8screen chip8screen;

};

/* sets the program counter (PC) to a specific memory address. The PC determines where the next instruction to execute is located in memory. */
void chip8_set_pc(struct chip8* chip8, short value);

/* increments the program counter (PC) by two bytes. Since each instruction in memory is two bytes long, this function allows the chip8 to move on to the next instruction during execution. */
void chip8_increment_pc(struct chip8* chip8);

/* initializes a new instance of the chip8 data structure. It sets default values for all registers, clears memory, and sets the program counter to its starting location. */
void chip8_init(struct chip8* chip8);

/* loads a binary buffer into memory, starting at the first memory address reserved for the game program. */
void chip8_load(struct chip8* chip8, const char* buf, size_t size);

/* decodes and executes an opcode instruction from memory. It uses various helper functions to determine how to manipulate registers, memory, and other parts of the virtual machine based on the current instruction. */
void chip8_exec(struct  chip8* chip8, unsigned short opcode);

/* handles opcodes that start with "0xF". These opcodes are used to manipulate the delay timer, sound timer, and the system's memory pointer. */
void chip8_exec_extended(struct  chip8* chip8, unsigned short opcode);

/* handles opcodes that start with "0x8". These opcodes are used to copy, compare, shift, rotate, and do other bitwise operations between registers. The x and y arguments specify which registers are being used. */
void chip8_exec_extended_eight(
    struct  chip8* chip8,
    unsigned short opcode,
    unsigned char x,
    unsigned char y
);

#endif