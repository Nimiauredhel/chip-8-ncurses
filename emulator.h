#ifndef EMULATOR_H
#define EMULATOR_H

#include "common.h"
#include "chip8_defs.h"
#include "data.h"
#include "renderer.h"
#include "parser.h"
#include "disassembler.h"

typedef enum Chip8KeyIndex
{
    CHIP8_KEY_0_IDX = 0,
    CHIP8_KEY_1_IDX = 1,
    CHIP8_KEY_2_IDX = 2,
    CHIP8_KEY_3_IDX = 3,
    CHIP8_KEY_4_IDX = 4,
    CHIP8_KEY_5_IDX = 5,
    CHIP8_KEY_6_IDX = 6,
    CHIP8_KEY_7_IDX = 7,
    CHIP8_KEY_8_IDX = 8,
    CHIP8_KEY_9_IDX = 9,
    CHIP8_KEY_A_IDX = 10,
    CHIP8_KEY_B_IDX = 11,
    CHIP8_KEY_C_IDX = 12,
    CHIP8_KEY_D_IDX = 13,
    CHIP8_KEY_E_IDX = 14,
    CHIP8_KEY_F_IDX = 15,
} Chip8KeyIndex_t;

typedef enum EmuKeyIndex
{
    EMU_KEY_QUIT_IDX = 0,
    EMU_KEY_RESET_IDX = 1,
    EMU_KEY_SPEED_UP_IDX = 2,
    EMU_KEY_SPEED_DOWN_IDX = 3,
    EMU_KEY_STEP_MODE_IDX = 4,
    EMU_KEY_STEP_ONE_IDX = 5,
} EmuKeyIndex_t;

typedef enum KeyBind
{
    KEY_NONE = ERR,

    EMU_KEY_QUIT_BIND = '~',       // ~ aka Shift-` 
    EMU_KEY_RESET_BIND = '|',      // Shift-\ -terminate the current instance and immediately launch another
    EMU_KEY_SPEED_UP_BIND = '+',   // Shift-Plus
    EMU_KEY_SPEED_DOWN_BIND = '_', // Shift-Minus 
    EMU_KEY_STEP_MODE_BIND = 'P',  // Shift-P - enter/exit step mode
    EMU_KEY_STEP_ONE_BIND = 'p',   // P - execute a single step in step mode

    CHIP8_KEY_1_BIND = '1',
    CHIP8_KEY_2_BIND = '2',
    CHIP8_KEY_3_BIND = '3',
    CHIP8_KEY_C_BIND = '4',
    CHIP8_KEY_4_BIND = 'q',
    CHIP8_KEY_5_BIND = 'w',
    CHIP8_KEY_6_BIND = 'e',
    CHIP8_KEY_D_BIND = 'r',
    CHIP8_KEY_7_BIND = 'a',
    CHIP8_KEY_8_BIND = 's',
    CHIP8_KEY_9_BIND = 'd',
    CHIP8_KEY_E_BIND = 'f',
    CHIP8_KEY_A_BIND = 'z',
    CHIP8_KEY_0_BIND = 'x',
    CHIP8_KEY_B_BIND = 'c',
    CHIP8_KEY_F_BIND = 'v',
} KeyBind_t;

bool run(Chip8_t *chip8);
void execute_instruction(Chip8_t *chip8, Chip8Instruction_t *instruction, WINDOW *window_chip8);

#endif
