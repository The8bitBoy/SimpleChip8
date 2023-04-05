#include "chip8memory.h"
#include <assert.h>

static void chip8_memory_in_bounds(int index)
{
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}
void chip8memory_set(struct chip8memory* memory, int index, char val)
{
    chip8_memory_in_bounds(index);
    memory->memory[index] = val;
}
unsigned char chip8memory_get(struct chip8memory* memory, int index)
{
    chip8_memory_in_bounds(index);
    return memory->memory[index];
}
unsigned short chip8memory_get_short(struct chip8memory* memory, int index)
{
    unsigned char byte1 = chip8memory_get(memory, index);
    unsigned char byte2 = chip8memory_get(memory, index + 1); //next byte
    return byte1 << 8 | byte2;
}