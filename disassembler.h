#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "common.h"
#include "parser.h"
#include "data.h"

void disassemble(Chip8_t *chip8, uint16_t program_end);

void print_instruction(uint8_t bytes[2], uint8_t nibbles[4], OpcodeIndex_t op_idx);

#endif
