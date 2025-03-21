#ifndef EMULATOR_H
#define EMULATOR_H

#include "common.h"
#include "chip8_defs.h"
#include "data.h"
#include "renderer.h"
#include "parser.h"
#include "disassembler.h"
#include "input.h"
#include "audio.h"

bool run(Chip8_t *chip8);
void execute_instruction(Chip8_t *chip8, Chip8Instruction_t *instruction, WINDOW *window_chip8);

#endif
