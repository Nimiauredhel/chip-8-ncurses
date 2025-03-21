#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "common.h"
#include "parser.h"
#include "data.h"

extern const char instruction_formats[CHIP8_INSTRUCTION_COUNT][32];

void disassemble(Chip8_t *chip8, uint16_t program_end);

void print_instruction(Chip8Instruction_t *instruction);
void mvwprintw_instruction(WINDOW *window_disassembly, int row, int col, Chip8Instruction_t *instruction);
void snprintf_instruction(char *dest, size_t maxlen, Chip8Instruction_t *instruction);

#endif
