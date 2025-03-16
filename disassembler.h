#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "common.h"
#include "parser.h"

void disassemble(uint8_t *program_ptr, size_t program_size);

void print_instruction(uint8_t bytes[2], uint8_t nibbles[4], OpcodeIndex_t op_idx);

#endif
