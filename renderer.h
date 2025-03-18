#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "data.h"
#include "chip8_defs.h"
#include "disassembler.h"

typedef struct DisplayLayout
{
    WINDOW *window_chip8;
    WINDOW *window_emu;
    WINDOW *window_disassembly;
    WINDOW *window_registers;
} DisplayLayout_t;

void init_display(DisplayLayout_t *layout);
void render_display(Chip8_t *chip8, WINDOW *window_chip8);
void render_disassembly(Chip8Instruction_t *instruction, WINDOW *window_disassembly);
void render_registers(Chip8Registers_t *registers, WINDOW *window_registers);
void render_emulator_state(EmulatorState_t *emu_state, WINDOW *window_emu);

#endif
