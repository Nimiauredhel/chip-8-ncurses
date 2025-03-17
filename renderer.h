#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "data.h"
#include "chip8_defs.h"

void init_display(Chip8_t *chip8, WINDOW **window_chip8, WINDOW **window_emu, WINDOW **window_disassembly);
void render_display(Chip8_t *chip8, WINDOW *window_chip8);
void render_disassembly(Chip8_t *chip8, WINDOW *window_disassembly);
void render_emulator_stats(Chip8_t *chip8, WINDOW *window_emu);

#endif
