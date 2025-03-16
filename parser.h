#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "instructions.h"

OpcodeIndex_t parse_instruction(uint8_t bytes[2], uint8_t nibbles[4]);

#endif
