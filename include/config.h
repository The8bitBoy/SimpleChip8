#ifndef CONFIG_H
#define CONCIG_H

//      +---------------+= 0xFFF (4095) End of Chip-8 RAM
//      |               |
//      |               |
//      |               |
//      |               |
//      |               |
//      | 0x200 to 0xFFF|
//      |     Chip-8    |
//      | Program / Data|
//      |     Space     |
//      |               |
//      |               |
//      |               |
//      +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
//      |               |
//      |               |
//      |               |
//      +---------------+= 0x200 (512) Start of most Chip-8 programs
//      | 0x000 to 0x1FF|
//      | Reserved for  |
//      |  interpreter  |
//      +---------------+= 0x000 (0) Start of Chip-8 RAM

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_SCREEN_WIDTH 62
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_WINDOW_MULTIPLIER 10


#define EMOLATUR_WINDOWS_TITLE "C8Emu"

#define CHIP_TOTAL_DATA_REGISTERS 16

#define CHIP8_TOTAOL_STACK_DEPTH 16
#define CHIP8_TOTAL_KEYS 16
#define CHIP8_CHARECTER_SET_LOAD_ADDRESS 0x00
#define CHIP8_PROGRAM_LOAD_ADDRESS 0x200
#define CHIP8_DEFAULT_SPRITE_HEIGHT 5
#endif