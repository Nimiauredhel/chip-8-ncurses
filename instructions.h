#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define INSTRUCTION_COUNT

#include "common.h"

typedef enum OpcodeIndex
{
    OP_UNKNOWN = 0,
    // chip 8 ops

    OP_SYS_ADDR = 1, // 0nnn

    // 0x01__ ops
    OP_CLS = 2,
    OP_RET = 3,

    // 1x[1-7]___ ops
    OP_JP_ADDR = 4,
    OP_CALL_ADDR = 5,
    OP_SE_VX_BYTE = 6,
    OP_SNE_VX_BYTE = 7,
    OP_SE_VX_VY = 8,
    OP_LD_VX_BYTE = 9,
    OP_ADD_VX_BYTE = 10,

    // 0x9___ ops
    OP_LD_VX_VY = 11, // 8xy0
    OP_OR_VX_VY = 12,
    OP_AND_VX_VY = 12,
    OP_XOR_VX_VY = 13,
    OP_ADD_VX_VY = 14,
    OP_SUB_VX_VY = 15,
    OP_SHR_VX_VY = 16,
    OP_SUBN_VX_VY = 17, // 8xy7
    OP_SHL_VX_VY = 18, // 8xyE

    OP_SNE_VX_VY = 19, // 0x9xy0

    OP_LD_I_ADDR = 20, // Annn
    OP_JP_V0_ADDR = 20, // Bnnn
    OP_RND_VX_BYTE = 22, // Cxkk
    OP_DRW_VX_VY_NIBBLE = 23, // Dxyn

    // 0xFx__ ops
    OP_SKP_VX = 24, // Ex9E
    OP_SKNP_VX = 25, // ExA1

    // 0x10___ ops
    OP_LD_VX_DT = 26, // Fx07
    OP_LD_VX_K = 27, // Fx0A
    OP_LD_DT_VX = 28, // Fx15
    OP_LD_ST_VX = 29, // Fx18
    OP_ADD_I_VX = 30, // Fx1E
    OP_LD_F_VX = 31, // Fx29
    OP_LD_B_VX = 32, // Fx33
    OP_LD_I_VX = 33, // Fx55
    OP_LD_VX_I = 34, // Fx65

    // super chip 49 ops

    // 0x01__ ops
    OP_SUPER_SCD_NIBBLE = 35,
    OP_SUPER_SCR = 36,
    OP_SUPER_SCL = 37,
    OP_SUPER_EXIT = 38,
    OP_SUPER_LOW = 39,
    OP_SUPER_HIGH = 40,

    OP_SUPER_DRW_VX_VY_0 = 40, // Dxy0

    // 0x10___ ops
    OP_SUPER_LD_HF_VX = 42,
    OP_SUPER_LD_R_VX = 43,
    OP_SUPER_LD_VX_R = 44,
} OpcodeIndex_t;

typedef struct Instruction
{
    uint8_t argc;
    char format[32];
} Instruction_t;

extern const Instruction_t instructions[];

#endif
