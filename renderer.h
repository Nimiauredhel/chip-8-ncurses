#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "data.h"
#include "chip8_defs.h"
#include "disassembler.h"
#include "input.h"

#define COLOR_PAIR_DISPLAY_ON 1
#define COLOR_PAIR_DISPLAY_OFF 2
#define COLOR_PAIR_TEXT_YELLOW 3
#define COLOR_PAIR_TEXT_RED 4
#define COLOR_PAIR_TEXT_MAGENTA 5
#define COLOR_PAIR_TEXT_CYAN 6
#define COLOR_PAIR_BG_YELLOW 10

void init_display(DisplayLayout_t *layout);
void render_display(Chip8_t *chip8, WINDOW *window_chip8);
void render_disassembly(Chip8Instruction_t *instruction, WINDOW *window_disassembly);
void render_registers(Chip8Registers_t *registers, WINDOW *window_registers);
void render_emulator_state(EmulatorState_t *emu_state, WINDOW *window_emu);

#endif
