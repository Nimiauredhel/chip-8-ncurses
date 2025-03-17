#ifndef DATA_H
#define DATA_H

#include "common.h"
#include "chip8_defs.h"

typedef union ComboRegister
{
    uint8_t bytes[2];
    uint16_t word;
} ComboRegister_t;

typedef struct Chip8
{
    uint8_t ST;
    uint8_t DT;
    uint8_t SP;
    uint8_t RNG;
    uint8_t V_REGS[16];
    uint16_t PC;
    uint16_t I_REG;
    ComboRegister_t EMU_TEMP[3]; // three 16 bit (or six 8 bit) registers I added for the instructions to use
    uint16_t STACK_RET[16];
    uint8_t DISPLAY[CHIP8_DISPLAY_SIZE]; // 32 x 8 bytes, every 8 bytes are a row, every bit is a pixel
    uint8_t RAM[4096];
} Chip8_t;

extern const uint8_t default_sprites[CHIP8_DEFAULT_SPRITES_SIZE];

Chip8_t *create_instance(char *rom_path);
void load_default_sprites(Chip8_t *chip8);
size_t load_rom(char *rom_path, uint8_t *dest_ptr);

#endif
