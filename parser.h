#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "chip8_defs.h"

#define PARSE_NNN_TO_U16(n1, n2, n3) ((uint16_t)(n1 << 8) + (n2 << 4) + n3)

typedef enum OpcodeIndex
{
    OP_UNKNOWN = 0,
    
    // *** Chip-8 Ops ***

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
    OP_AND_VX_VY = 13,
    OP_XOR_VX_VY = 14,
    OP_ADD_VX_VY = 15,
    OP_SUB_VX_VY = 16,
    OP_SHR_VX_VY = 17,
    OP_SUBN_VX_VY = 18, // 8xy7
    OP_SHL_VX_VY = 19, // 9xyE

    OP_SNE_VX_VY = 20, // 0x9xy0

    OP_LD_I_ADDR = 21, // Annn
    OP_JP_V0_ADDR = 22, // Bnnn
    OP_RND_VX_BYTE = 23, // Cxkk
    OP_DRW_VX_VY_NIBBLE = 24, // Dxyn

    // 0xFx__ ops
    OP_SKP_VX = 25, // Ex9E
    OP_SKNP_VX = 26, // ExA1

    // 0x10___ ops
    OP_LD_VX_DT = 27, // Fx07
    OP_LD_VX_K = 28, // Fx0A
    OP_LD_DT_VX = 29, // Fx15
    OP_LD_ST_VX = 30, // Fx18
    OP_ADD_I_VX = 31, // Fx1E
    OP_LD_F_VX = 32, // Fx29
    OP_LD_B_VX = 33, // Fx33
    OP_LD_I_VX = 34, // Fx55
    OP_LD_VX_I = 35, // Fx65

    // *** Super Chip-48 Ops ***

    // 0x01__ ops
    OP_SUPER_SCD_NIBBLE = 36,
    OP_SUPER_SCR = 37,
    OP_SUPER_SCL = 38,
    OP_SUPER_EXIT = 39,
    OP_SUPER_LOW = 40,
    OP_SUPER_HIGH = 41,

    OP_SUPER_DRW_VX_VY_0 = 42, // Dxy0

    // 0x10___ ops
    OP_SUPER_LD_HF_VX = 43,
    OP_SUPER_LD_R_VX = 44,
    OP_SUPER_LD_VX_R = 45,
} OpcodeIndex_t;

typedef enum InstructionSchema
{
    OPSCH_NONE = 0,
    OPSCH_NNN = 1, // 12 bit value of nibbles 1, 2, 3
    OPSCH_XKK = 2,
    OPSCH_XY_ = 3,
    OPSCH_XYN = 4,
    OPSCH_X__ = 5,
    OPSCH___N = 6,
} InstructionSchema_t;

typedef struct Chip8Instruction
{
    OpcodeIndex_t op_idx;
    uint8_t nibble0 : 4;
    uint8_t nibble1 : 4;
    uint8_t nibble2 : 4;
    uint8_t nibble3 : 4;
    uint8_t bytes[2];
} Chip8Instruction_t;

extern const InstructionSchema_t instruction_schemas[CHIP8_INSTRUCTION_COUNT];

void decode_instruction(Chip8Instruction_t *instruction);

#endif
