#include "parser.h"

const InstructionSchema_t instruction_schemas[CHIP8_INSTRUCTION_COUNT] =
{
    // any unknown/invalid instructions, which are currently ignored
    OPSCH_NONE, 

    // chip 8 instructions
    OPSCH_NONE, // 0nnn SYS addr - legacy, ignore
    OPSCH_NONE, // CLS - clear screen
    OPSCH_NONE, // RET - return from subroutine

    OPSCH_NNN, // 1nnn JP addr - set PC to nnn
    OPSCH_NNN, // 2nnn CALL addr - call subroutine
    OPSCH_XKK, // 3xkk SE Vx, byte - skip next if Vx == kk
    OPSCH_XKK,
    OPSCH_XY_,
    OPSCH_XKK,
    OPSCH_XKK, // 7xkk ADD Vx, byte - set Vx = Vx + kk

    OPSCH_XY_, // 8xy0 LD Vx, Vy - store value of Vy in Vx
    OPSCH_XY_,
    OPSCH_XY_,
    OPSCH_XY_,
    OPSCH_XY_,
    OPSCH_XY_,
    OPSCH_XY_,
    OPSCH_XY_,
    OPSCH_XY_, // 8xye SHL Vx {, Vy}

    OPSCH_XY_, // 9xy0 SNE Vx, Vy

    OPSCH_NNN, // Annn LD I, addr
    OPSCH_NNN, // Bnnn JP V0, addr
    OPSCH_XKK, // Cxkk RND Vx, byte
    OPSCH_XYN, // Dxyn DRW Vx, Vy, nibble

    OPSCH_X__, // Ex9E SKP Vx
    OPSCH_X__, // ExA1 SKNP Vx

    OPSCH_X__, // Fx07 LD Vx, DT
    OPSCH_X__, 
    OPSCH_X__, 
    OPSCH_X__, 
    OPSCH_X__, 
    OPSCH_X__, 
    OPSCH_X__, 
    OPSCH_X__, 
    OPSCH_X__, 

    // super chip 48 instructions
    OPSCH___N, // SCD nibble
    OPSCH_NONE,
    OPSCH_NONE,
    OPSCH_NONE,
    OPSCH_NONE,
    OPSCH_NONE,

    OPSCH_XY_,
    OPSCH_X__,
    OPSCH_X__,
    OPSCH_X__,
};

void decode_instruction(Chip8Instruction_t *instruction)
{
    // macroing this because changing my mind ONCE was tedious enough
#define RETURN_OPCODE(opcode) instruction->op_idx = opcode; return

    // the 0x00__ instructions
    if (instruction->bytes[0] == 0x00)
    {
        switch(instruction->nibble2)
        {
            // chip 8
            case 0xE:
                switch(instruction->nibble3)
                {
                    case 0x0:
                        RETURN_OPCODE(OP_CLS);
                    case 0xE:
                        RETURN_OPCODE(OP_RET);
                    default:
                        RETURN_OPCODE(OP_UNKNOWN);
                }
            // super chip 48
            case 0xC:
                RETURN_OPCODE(OP_SUPER_SCD_NIBBLE); // 00Cn
            case 0xF:
                switch (instruction->nibble3)
                {
                    case 0xB:
                        RETURN_OPCODE(OP_SUPER_SCR);
                    case 0xC:
                        RETURN_OPCODE(OP_SUPER_SCL);
                    case 0xD:
                        RETURN_OPCODE(OP_SUPER_EXIT);
                    case 0xE:
                        RETURN_OPCODE(OP_SUPER_LOW);
                    case 0xF:
                        RETURN_OPCODE(OP_SUPER_HIGH);
                    default:
                        RETURN_OPCODE(OP_UNKNOWN);
                }
            default:
                RETURN_OPCODE(OP_UNKNOWN);
        }
    }

    // everything else
    switch (instruction->nibble0)
    {
        case 0x0:
            RETURN_OPCODE(OP_SYS_ADDR); // 0nnn
        case 0x1:
            RETURN_OPCODE(OP_JP_ADDR); // 1nnn
        case 0x2:
            RETURN_OPCODE(OP_CALL_ADDR); // 2nnn etc..
        case 0x3:
            RETURN_OPCODE(OP_SE_VX_BYTE);
        case 0x4:
            RETURN_OPCODE(OP_SNE_VX_BYTE);
        case 0x5:
            RETURN_OPCODE(OP_SE_VX_VY);
        case 0x6:
            RETURN_OPCODE(OP_LD_VX_BYTE);
        case 0x7:
            RETURN_OPCODE(OP_ADD_VX_BYTE);
        // in 0x8xy_ opcodes, last nibble determines operand
        case 0x8:
            switch (instruction->nibble3)
            {
                case 0x0:
                    RETURN_OPCODE(OP_LD_VX_VY);
                case 0x1:
                    RETURN_OPCODE(OP_OR_VX_VY);
                case 0x2:
                    RETURN_OPCODE(OP_AND_VX_VY);
                case 0x3:
                    RETURN_OPCODE(OP_XOR_VX_VY);
                case 0x4:
                    RETURN_OPCODE(OP_ADD_VX_VY);
                case 0x5:
                    RETURN_OPCODE(OP_SUB_VX_VY);
                case 0x6:
                    RETURN_OPCODE(OP_SHR_VX_VY);
                case 0x7:
                    RETURN_OPCODE(OP_SUBN_VX_VY);
                case 0xE:
                    RETURN_OPCODE(OP_SHL_VX_VY);
                default:
                    RETURN_OPCODE(OP_UNKNOWN);
            }
        case 0x9:
            RETURN_OPCODE(OP_SNE_VX_VY);
        case 0xA:
            RETURN_OPCODE(OP_LD_I_ADDR);
        case 0xB:
            RETURN_OPCODE(OP_JP_V0_ADDR);
        case 0xC:
            RETURN_OPCODE(OP_RND_VX_BYTE);
        case 0xD:
            RETURN_OPCODE(instruction->nibble3 == 0x0
            ? OP_SUPER_DRW_VX_VY_0
            : OP_DRW_VX_VY_NIBBLE);
        case 0xE:
            switch (instruction->bytes[1])
            {
                case 0x9E:
                    RETURN_OPCODE(OP_SKP_VX);
                case 0xA1:
                    RETURN_OPCODE(OP_SKNP_VX);
                default:
                    RETURN_OPCODE(OP_UNKNOWN);
            }
        case 0xF:
            switch(instruction->bytes[1])
            {
                case 0x07:
                    RETURN_OPCODE(OP_LD_VX_DT);
                case 0x0A:
                    RETURN_OPCODE(OP_LD_VX_K);
                case 0x15:
                    RETURN_OPCODE(OP_LD_DT_VX);
                case 0x18:
                    RETURN_OPCODE(OP_LD_ST_VX);
                case 0x1E:
                    RETURN_OPCODE(OP_ADD_I_VX);
                case 0x29:
                    RETURN_OPCODE(OP_LD_F_VX);
                case 0x33:
                    RETURN_OPCODE(OP_LD_B_VX);
                case 0x55:
                    RETURN_OPCODE(OP_LD_I_VX);
                case 0x65:
                    RETURN_OPCODE(OP_LD_VX_I);
                // super chip 48
                case 0x30:
                    RETURN_OPCODE(OP_SUPER_LD_HF_VX);
                case 0x75:
                    RETURN_OPCODE(OP_SUPER_LD_R_VX);
                case 0x85:
                    RETURN_OPCODE(OP_SUPER_LD_VX_R);
                default:
                    RETURN_OPCODE(OP_UNKNOWN);
            }
        default:
            RETURN_OPCODE(OP_UNKNOWN);
    }
#undef RETURN_OPCODE
}
