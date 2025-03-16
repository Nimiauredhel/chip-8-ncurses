#include "parser.h"

OpcodeIndex_t parse_instruction(uint8_t bytes[2], uint8_t nibbles[4])
{
    if (bytes[0] == 0x00)
    {
        switch(nibbles[2])
        {
            // chip 8
            case 0xE:
                switch(nibbles[3])
                {
                    case 0x0:
                        return OP_CLS;
                    case 0xE:
                        return OP_RET;
                    default:
                        return OP_UNKNOWN;
                }
            // super chip 48
            case 0xC:
                return OP_SUPER_SCD_NIBBLE; // 00Cn
            case 0xF:
                switch (nibbles[3])
                {
                    case 0xB:
                        return OP_SUPER_SCR;
                    case 0xC:
                        return OP_SUPER_SCL;
                    case 0xD:
                        return OP_SUPER_EXIT;
                    case 0xE:
                        return OP_SUPER_LOW;
                    case 0xF:
                        return OP_SUPER_HIGH;
                    default:
                        return OP_UNKNOWN;
                }
            default:
                return OP_UNKNOWN;
        }
    }
        
    switch (nibbles[0])
    {
        case 0x0:
            return OP_SYS_ADDR; // 0nnn
        case 0x1:
            return OP_JP_ADDR; // 1nnn
        case 0x2:
            return OP_CALL_ADDR; // 2nnn etc..
        case 0x3:
            return OP_SE_VX_BYTE;
        case 0x4:
            return OP_SNE_VX_BYTE;
        case 0x5:
            return OP_SE_VX_VY;
        case 0x6:
            return OP_LD_VX_BYTE;
        case 0x7:
            return OP_ADD_VX_BYTE;
        // in 0x8xy_ opcodes, last nibble determines operand
        case 0x8:
            switch (nibbles[3])
            {
                case 0x0:
                    return OP_LD_VX_VY;
                case 0x1:
                    return OP_OR_VX_VY;
                case 0x2:
                    return OP_AND_VX_VY;
                case 0x3:
                    return OP_XOR_VX_VY;
                case 0x4:
                    return OP_ADD_VX_VY;
                case 0x5:
                    return OP_SUB_VX_VY;
                case 0x6:
                    return OP_SHR_VX_VY;
                case 0x7:
                    return OP_SUBN_VX_VY;
                case 0xE:
                    return OP_SHL_VX_VY;
                default:
                    return OP_UNKNOWN;
            }
        case 0x9:
            return OP_SNE_VX_VY;
        case 0xA:
            return OP_LD_I_ADDR;
        case 0xB:
            return OP_JP_V0_ADDR;
        case 0xC:
            return OP_RND_VX_BYTE;
        case 0xD:
            return nibbles[3] == 0x0
            ? OP_SUPER_DRW_VX_VY_0
            : OP_DRW_VX_VY_NIBBLE;
        case 0xE:
            switch (bytes[1])
            {
                case 0x9E:
                    return OP_SKP_VX;
                case 0xA1:
                    return OP_SKNP_VX;
                default:
                    return OP_UNKNOWN;
            }
        case 0xF:
            switch(bytes[1])
            {
                case 0x07:
                    return OP_LD_VX_DT;
                case 0x0A:
                    return OP_LD_VX_K;
                case 0x15:
                    return OP_LD_DT_VX;
                case 0x18:
                    return OP_LD_ST_VX;
                case 0x1E:
                    return OP_ADD_I_VX;
                case 0x29:
                    return OP_LD_F_VX;
                case 0x33:
                    return OP_LD_B_VX;
                case 0x55:
                    return OP_LD_I_VX;
                case 0x65:
                    return OP_LD_VX_I;
                // super chip 48
                case 0x30:
                    return OP_SUPER_LD_HF_VX;
                case 0x75:
                    return OP_SUPER_LD_R_VX;
                case 0x85:
                    return OP_SUPER_LD_VX_R;
                default:
                    return OP_UNKNOWN;
            }

        default:
            return OP_UNKNOWN;
    }
}
