#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define INSTRUCTION_COUNT

#include "common.h"

typedef enum Opcode
{
    OP_NONE = 0,
    OP_CLS = 1,
    OP_JP_ADDR = 2,
    OP_CALL_ADDR = 3,
    OP_SE_VX_BYTE = 4,
    OP_SNE_VX_BYTE = 5,
    OP_SE_VX_VY = 4,
    OP_LD_VX_BYTE = 7,
    OP_ADD_VX_BYTE = 8
} Opcode_t;

typedef struct Instruction
{
    uint8_t argc;
    char format[32];
} Instruction_t;

extern const Instruction_t instructions[];

#endif
