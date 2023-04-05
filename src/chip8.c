#include <memory.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "chip8.h" 
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

const char chip8_default_charecter_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10, 
    0xF0, 0x80, 0xF0, 0x10, 0xF0, 
    0xF0, 0x80, 0xF0, 0x90, 0xF0, 
    0xF0, 0x10, 0x20, 0x40, 0x40, 
    0xF0, 0x90, 0xF0, 0x90, 0xF0, 
    0xF0, 0x90, 0xF0, 0x10, 0xF0, 
    0xF0, 0x90, 0xF0, 0x90, 0x90, 
    0xE0, 0x90, 0xE0, 0x90, 0xE0, 
    0xF0, 0x80, 0x80, 0x80, 0xF0, 
    0xE0, 0x90, 0x90, 0x90, 0xE0, 
    0xF0, 0x80, 0xF0, 0x80, 0xF0, 
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

const char chip8_default_key_map[CHIP8_TOTAL_KEYS] = {
    KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7,
    KEY_8,KEY_9, KEY_a, KEY_b, KEY_c, KEY_d, KEY_e, KEY_f
};

void chip8_init(struct chip8* chip8)
{
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(
        &chip8->memory.memory,
        chip8_default_charecter_set,
        sizeof(chip8_default_charecter_set)
        );
}

void chip8_set_pc(struct chip8* chip8, short value){
    chip8->registers.PC = value;
}

void chip8_increment_pc(struct chip8* chip8){
    short inc_value = chip8->registers.PC;
    inc_value += 1;
    chip8->registers.PC = inc_value;
}

void chip8_load(struct chip8* chip8, const char* buf, size_t size)
{
    assert((size + CHIP8_PROGRAM_LOAD_ADDRESS) <= CHIP8_MEMORY_SIZE);

    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
    chip8_set_pc(chip8, CHIP8_PROGRAM_LOAD_ADDRESS);
}

static char chip8_wait_for_key_press(struct chip8* chip8)
{
    SDL_Event event;
    while (SDL_WaitEvent(&event))    
    {
        if (event.type != SDL_KEYDOWN)
        {
            continue;
        }
        char c = event.key.keysym.sym;
        char mapped = chip8_kryboard_map(chip8_default_key_map, c);
        if (mapped >= 0)
            return mapped;
        continue;
    }
    return -1;
}

void chip8_exec_extended_F(
    struct chip8* chip8,
    unsigned short opcode,
    unsigned char x,
    unsigned char y
    )
{
    switch (opcode & 0x00ff)
    {
        // LD Vx, DT
        case 0x07:
            chip8->registers.V[x] = chip8->registers.delay_timer;
            break;
         // LD Vx, key
        case 0x0A:
            char pressed_key = chip8_wait_for_key_press(chip8);
            chip8->registers.V[x] = pressed_key;
            break;
        // LD DT, Vx
        case 0x15:
            chip8->registers.delay_timer = chip8->registers.V[x];
            break;
        // LD ST, Vx
        case 0x18:
            chip8->registers.sound_timer = chip8->registers.V[x];
            break;
        // ADD I, Vx
        case 0x1E:
            chip8->registers.I += chip8->registers.V[x];
            break;
        // LD F, Vx
        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
            break;
        // LD B, Vx
        case 0x33:
        {
            unsigned char hundreds = chip8->registers.V[x] / 100;
            chip8memory_set(&chip8->memory, chip8->registers.I, hundreds);
            unsigned char tens = chip8->registers.V[x] / 10 % 10;
            chip8memory_set(&chip8->memory, chip8->registers.I+1, tens);
            unsigned char units = chip8->registers.V[x]% 10;
            chip8memory_set(&chip8->memory, chip8->registers.I+2, units);
        }
            break;

        // LD [I], Vx
        case 0x55:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8memory_set(&chip8->memory, chip8->registers.I+i, chip8->registers.V[i]);
            }
            break;
        }
        // LD Vx, [I]
        case 0x65:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8->registers.V[i] = chip8memory_get(&chip8->memory, chip8->registers.I+i);
            }
        }
            break;
        default:
            break;
    }
}
void chip8_exec_extended_eight(
    struct chip8* chip8,
    unsigned short opcode,
    unsigned char x,
    unsigned char y
    )
{
    unsigned char z = opcode & 0x000f;
    unsigned short temp = 0;
    switch (z)
    {
    //LD Vx, Vy
    case 0x00:
        chip8->registers.V[x] = chip8->registers.V[y];
        break;
    //OR Vx, Vy
    case 0x01:
        chip8->registers.V[x] |= chip8->registers.V[y];
        break;
    //AND Vx, Vy
    case 0x02:
        chip8->registers.V[x] &= chip8->registers.V[y];
        break;
    //XOR Vx, Vy
    case 0x03:
        chip8->registers.V[x] ^= chip8->registers.V[y];
        break;
    //ADD Vx, Vy
    case 0x04:
        temp = chip8->registers.V[x] + chip8->registers.V[y];
        chip8->registers.V[0x0f] = false;
        if (temp > 0xff)
        {
            chip8->registers.V[0x0f] = true;
        }
        chip8->registers.V[x] = temp;
        break;
    //SUB Vx, Vy
    case 0x05:
        chip8->registers.V[0x0f] = false;
        if (chip8->registers.V[x] > chip8->registers.V[y])
        {
            chip8->registers.V[0x0f] = true;
        }
        temp = chip8->registers.V[x] - chip8->registers.V[y];
        chip8->registers.V[x] = temp;
        break;
    //SHR Vx {, Vy}
    case 0x06:
        chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;
        chip8->registers.V[x] /= 2;
        break; 
    //SUBN Vx, Vy
    case 0x07:
        chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
        chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;  
    //SHL Vx 
    case 0x0E:
        chip8->registers.V[0x0f] = (chip8->registers.V[x] & 0x80);
        chip8->registers.V[x] *= 2;
        break;  
    default:
        break;
    }
}
void chip8_exec_extended(struct chip8* chip8, unsigned short opcode)
{
    unsigned short nnn = opcode & 0x0fff;
    unsigned short kk = opcode & 0x00ff;
    unsigned short n = opcode & 0x000f;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    
    switch (opcode & 0xf000)
    {
        //JP
        case 0x1000:
            chip8_set_pc(chip8, nnn);
            break;
        //CALL
        case 0x2000:
            chip8_stack_push(chip8, chip8->registers.PC);
            chip8_set_pc(chip8, nnn);
            break;
        //SE Vx, byte
        case 0x3000:
            if (chip8->registers.V[x] == kk)
            {
                chip8_increment_pc(chip8);
                chip8_increment_pc(chip8);
            }
            break;
        //SNE Vx, byte
        case 0x4000:
            if (chip8->registers.V[x] != kk)
            {
                chip8_increment_pc(chip8);
                chip8_increment_pc(chip8);
            }
            break;
        //SE Vx, Vy
        case 0x5000:
            if (chip8->registers.V[x] == chip8->registers.V[y])
            {
                chip8_increment_pc(chip8);
                chip8_increment_pc(chip8);
            }
            break;
        //LD Vx, byte
        case 0x6000:
            chip8->registers.V[x] = kk;
            break;
        //ADD Vx, byte
        case 0x7000:
            chip8->registers.V[x] += kk;
            break;
        //Extend
        case 0x8000:
            chip8_exec_extended_eight(chip8, opcode, x, y);
            break;
        //SNE Vx, Vy
        case 0x9000:
            if (chip8->registers.V[x] != chip8->registers.V[y])
            {
                chip8_increment_pc(chip8);
                chip8_increment_pc(chip8);
            }
            break;
        //LD I, addr
        case 0xA000:
            chip8->registers.I = nnn;
            break;
        case 0xB000:
            chip8->registers.PC = chip8->registers.V[0x00] + nnn;
            break;
        //RND Vx, byte
        case 0xC000:
            srand(clock());
            chip8->registers.V[x] = (rand() % 255) & kk;
            break;
        //DRW Vx, Vy 
        case 0xD000:
        {   
            const char* sprite = (const char*) &chip8->memory.memory[chip8->registers.I];
            char res = chip8screen_draw_sprite(
                &chip8->chip8screen,
                chip8->registers.V[x],
                chip8->registers.V[y],
                sprite,
                n
                );
            chip8->registers.V[0x0f] = res;
        }
            break;
        //Keyboard Events
        case 0xE000:
            {
                unsigned short subcode = opcode & 0x00ff;
                if (subcode == 0x9e)
                {
                    //SKP Vx
                    if (chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x]))
                    {
                        chip8_increment_pc(chip8);
                        chip8_increment_pc(chip8);
                    } 
                }
                else if (subcode == 0xa1)
                {
                    //SKNP Vx
                    if (!chip8_keyboard_is_down(&chip8->keyboard, chip8->registers.V[x]))
                    {
                        chip8_increment_pc(chip8);
                        chip8_increment_pc(chip8);
                    }
                }
            }
            break;
        
        case 0xF000:
            chip8_exec_extended_F(chip8, opcode, x, y);
            break;
        default:
            break;
        }
}

void chip8_exec(struct chip8* chip8, unsigned short opcode)
{
    switch (opcode)
    {
        //CLS: clear screen
        case 0x00E0:
            chip8screen_clear(&chip8->chip8screen);
            break;
        //RET
        case 0x00EE:
            chip8_set_pc(chip8, chip8_stack_pop(chip8));
            break;
        default:
            chip8_exec_extended(chip8, opcode);
            break;
    }
}
