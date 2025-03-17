#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "instructions.h"

OpcodeIndex_t parse_instruction(uint8_t bytes[2], uint8_t nibbles[4]);

uint16_t parse_nnn(uint8_t n1, uint8_t n2, uint8_t n3);

#endif
