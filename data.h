#ifndef DATA_H
#define DATA_H

#include "common.h"

typedef struct Chip8
{
    bool quit;
    uint8_t ST;
    uint8_t DT;
    uint8_t V_REGS[16];
    uint16_t I_REG;
    uint16_t PC;
    uint8_t SP;
    uint16_t STACK_RET[16];
    uint16_t SPRITES[16];
    uint8_t RAM[4096];
} Chip8_t;

extern const uint8_t default_sprites[80];

void load_sprites(Chip8_t *chip8, const uint8_t sprites[80], uint16_t starting_address);

#endif
