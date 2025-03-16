#include "instructions.h"

const Instruction_t instructions[46] =
{
    { 1, "UNKNOWN (%4x)" }, // 0nnn SYS addr - legacy, ignore
                      //
    // chip 8 instructions
    { OPSCH_NONE, "SYS %4x" }, // 0nnn SYS addr - legacy, ignore
    { OPSCH_NONE, "CLS" }, // CLS - clear screen
    { OPSCH_NONE, "RET" }, // RET - return from subroutine

    { OPSCH_NNN, "JP %d" }, // 1nnn JP addr - set PC to nnn
    { OPSCH_NNN, "CALL %d" }, // 2nnn CALL addr - call subroutine
    { OPSCH_XKK, "SE V%u, %d" }, // 3xkk SE Vx, byte - skip next if Vx == kk
    { OPSCH_XKK, "SNE V%u, %d" },
    { OPSCH_XY_, "SE V%u, V%u" },
    { OPSCH_XKK, "LD V%u, %d" },
    { OPSCH_XKK, "ADD V%u, %d" }, // 7xkk ADD Vx, byte - set Vx = Vx + kk

    { OPSCH_XY_, "LD V%u, V%u" }, // 8xy0 LD Vx, Vy - store value of Vy in Vx
    { OPSCH_XY_, "OR V%u, V%u" },
    { OPSCH_XY_, "AND V%u, V%u" },
    { OPSCH_XY_, "XOR V%u, V%u" },
    { OPSCH_XY_, "ADD V%u, V%u" },
    { OPSCH_XY_, "SUB V%u, V%u" },
    { OPSCH_XY_, "SHR V%u {, V%u}" },
    { OPSCH_XY_, "SUBN V%u, V%u" },
    { OPSCH_XY_, "SHL V%u {, V%u}" }, // 8xye SHL Vx {, Vy}

    { OPSCH_XY_, "SNE V%u, V%u" }, // 9xy0 SNE Vx, Vy

    { OPSCH_NNN, "LD I, %d" }, // Annn LD I, addr
    { OPSCH_NNN, "JP V0, %d" }, // Bnnn JP V0, addr
    { OPSCH_XKK, "RND V%u, %d" }, // Cxkk RND Vx, byte
    { OPSCH_XYN, "DRW V%u, V%u, %d" }, // Dxyn DRW Vx, Vy, nibble

    { OPSCH_X__, "SKP Vx" }, // Ex9E SKP Vx
    { OPSCH_X__, "SKNP Vx" }, // ExA1 SKNP Vx

    { OPSCH_X__, "LD V%u, DT" }, // Fx07 LD Vx, DT
    { OPSCH_X__, "LD V%u, K" },
    { OPSCH_X__, "LD DT, V%u" },
    { OPSCH_X__, "LD ST, V%u" },
    { OPSCH_X__, "ADD I, V%u" },
    { OPSCH_X__, "LD F, V%u" },
    { OPSCH_X__, "LD B, V%u" },
    { OPSCH_X__, "LD [I], V%u" },
    { OPSCH_X__, "LD V%u, [I]" },

    // super chip 48 instructions
    { OPSCH___N, "SCD %d" }, // SCD nibble
    { OPSCH_NONE, "SCR" },
    { OPSCH_NONE, "SCL" },
    { OPSCH_NONE, "EXIT" },
    { OPSCH_NONE, "LOW" },
    { OPSCH_NONE, "HIGH" },

    { OPSCH_XY_, "DRW V%u, V%u, 0" },
    { OPSCH_X__, "LD HF, V%u" },
    { OPSCH_X__, "LD R, V%u" },
    { OPSCH_X__, "LD V%u, R" },
};

void execute_instruction(Chip8_t *chip8, uint8_t bytes[2], uint8_t nibbles[4], OpcodeIndex_t op_idx)
{
    switch (op_idx)
    {
        case OP_UNKNOWN:
        case OP_SYS_ADDR:
        case OP_CLS:
        case OP_RET:
        case OP_JP_ADDR:
        case OP_CALL_ADDR:
        case OP_SE_VX_BYTE:
        case OP_SNE_VX_BYTE:
        case OP_SE_VX_VY:
        case OP_LD_VX_BYTE:
        case OP_ADD_VX_BYTE:
        case OP_LD_VX_VY:
        case OP_OR_VX_VY:
        case OP_AND_VX_VY:
        case OP_XOR_VX_VY:
        case OP_ADD_VX_VY:
        case OP_SUB_VX_VY:
        case OP_SHR_VX_VY:
        case OP_SUBN_VX_VY:
        case OP_SHL_VX_VY:
        case OP_SNE_VX_VY:
        case OP_LD_I_ADDR:
        case OP_JP_V0_ADDR:
        case OP_RND_VX_BYTE:
        case OP_DRW_VX_VY_NIBBLE:
        case OP_SKP_VX:
        case OP_SKNP_VX:
        case OP_LD_VX_DT:
        case OP_LD_VX_K:
        case OP_LD_DT_VX:
        case OP_LD_ST_VX:
        case OP_ADD_I_VX:
        case OP_LD_F_VX:
        case OP_LD_B_VX:
        case OP_LD_I_VX:
        case OP_LD_VX_I:
        case OP_SUPER_SCD_NIBBLE:
        case OP_SUPER_SCR:
        case OP_SUPER_SCL:
        case OP_SUPER_EXIT:
        case OP_SUPER_LOW:
        case OP_SUPER_HIGH:
        case OP_SUPER_DRW_VX_VY_0:
        case OP_SUPER_LD_HF_VX:
        case OP_SUPER_LD_R_VX:
        case OP_SUPER_LD_VX_R:
      break;
    }
}
