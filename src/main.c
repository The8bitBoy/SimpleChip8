#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
#include "chip8.h"
#include "chip8keyboard.h"

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3,
    SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

void draw_screen(struct chip8* chip8, SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++)
        {
            if (chip8screen_is_set(&chip8->chip8screen, x, y))
            {
                SDL_Rect r;
                r.x = x * CHIP8_WINDOW_MULTIPLIER;
                r.y = y * CHIP8_WINDOW_MULTIPLIER;
                r.w = CHIP8_WINDOW_MULTIPLIER;
                r.h = CHIP8_WINDOW_MULTIPLIER;
                SDL_RenderFillRect(renderer, &r);
            }
        }   
    }
    SDL_RenderPresent(renderer);
}

void print_memory(void* ptr, int size) {
    unsigned char* p = (unsigned char*) ptr;
    for (int i = 0; i < size; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (p[i] >> j) & 1);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char* args[])
{

    if (argc < 2)
    {
        printf("Please pass a Chip8 file to execute!");
        return -1;
    }
    
    const char* file_name = args[1];

    FILE* chip8_file = fopen(file_name, "rb");
    if (!chip8_file)
    {
        printf("Failed to open chip8 file! \nMake sure its a chip8 excutable file.\n");
        return -1;
    }
    
    fseek(chip8_file, 0, SEEK_END);
    long file_size = ftell(chip8_file);
    fseek(chip8_file, 0, SEEK_SET);

    char file_buf[file_size];

    int res = fread(file_buf, file_size, 1, chip8_file);
    if (res!=1){
        printf("Failed to read the input file!");
        return -1;
    }

    struct chip8 chip8;
    chip8_init(&chip8);

    chip8_load(&chip8, file_buf, file_size);
    
    // chip8.registers.V[0] = 10;
    // chip8.registers.V[1] = 10;
    // chip8_exec(&chip8, 0xf00A);
    // printf("%x \n%X", chip8.registers.V[0], chip8.registers.V[0x0f]);
    // print_memory(&chip8.memory.memory, CHIP8_MEMORY_SIZE);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMOLATUR_WINDOWS_TITLE,
        SDL_WINDOWPOS_UNDEFINED_MASK, 
        SDL_WINDOWPOS_UNDEFINED_MASK,
        CHIP8_WINDOW_MULTIPLIER * CHIP8_SCREEN_WIDTH,
        CHIP8_WINDOW_MULTIPLIER * CHIP8_SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                    goto out;
                break;
            case SDL_KEYDOWN:
                {
                    char key = event.key.keysym.sym;
                    int correct_key =  chip8_kryboard_map(keyboard_map, key);
                    if (correct_key != -1)
                    {
                        chip8_keyboard_down(&chip8.keyboard, correct_key);
                    }
                    // for (int i = 0; i <= 0x0f; i++)
                    //     {
                    //         printf("%d:%x \t", i, chip8.keyboard.keyboard[i]);
                    //     }
                    // printf("\n");
               }
            break;
            case SDL_KEYUP:
                {
                    char key = event.key.keysym.sym;
                    int correct_key =  chip8_kryboard_map(keyboard_map, key);
                    if (correct_key != -1)
                    {
                        chip8_keyboard_up(&chip8.keyboard, correct_key);
                    }
                }            
                break;
            default:
                break;
            }
        }
        
        draw_screen(&chip8, renderer);

        if (chip8.registers.delay_timer > 0)
        {
            Sleep(10);
            chip8.registers.delay_timer -=1;
        }

        if (chip8.registers.sound_timer > 0){
            Beep(1000, 10 * chip8.registers.sound_timer);
            chip8.registers.sound_timer = 0;
        }

        unsigned short opcode = chip8memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8_increment_pc(&chip8);
        chip8_increment_pc(&chip8);
        chip8_exec(&chip8, opcode);
        
 }

out:
    SDL_DestroyWindow(window);
    return 0;
}