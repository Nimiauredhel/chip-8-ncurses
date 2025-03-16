#include "common.h"
#include "instructions.h"
#include "disassembler.h"

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
    static uint16_t temp = 0;

    switch (op_idx)
    {
        case OP_UNKNOWN:
        case OP_SYS_ADDR:
            break;
        case OP_CLS:
            explicit_bzero(chip8->DISPLAY, 256);
            break;
        case OP_RET:
            chip8->PC = chip8->STACK_RET[chip8->SP];
            chip8->PC -= 2;
            chip8->ST--;
            break;
        case OP_CALL_ADDR:
            chip8->SP++;
            chip8->STACK_RET[chip8->SP] = chip8->PC;
            // intentional fall through!
        case OP_JP_ADDR:
            chip8->PC =
            (nibbles[1] << 8)
            + (nibbles[2] << 4)
            + nibbles[3];
            chip8->PC -= 2;
            break;
        case OP_SE_VX_BYTE:
            if (chip8->V_REGS[nibbles[1]]
            == bytes[1]) chip8->PC += 2;
            break;
        case OP_SNE_VX_BYTE:
            if (chip8->V_REGS[nibbles[1]]
            != bytes[1]) chip8->PC += 2;
            break;
        case OP_SE_VX_VY:
            if (chip8->V_REGS[nibbles[1]]
            == chip8->V_REGS[nibbles[2]]) chip8->PC += 2;
            break;
        case OP_LD_VX_BYTE:
            chip8->V_REGS[nibbles[1]]
            = bytes[1];
            break;
        case OP_ADD_VX_BYTE:
            chip8->V_REGS[nibbles[1]]
            += bytes[1];
            break;
        case OP_LD_VX_VY:
            chip8->V_REGS[nibbles[1]]
            = chip8->V_REGS[nibbles[2]];
            break;
        case OP_OR_VX_VY:
            chip8->V_REGS[nibbles[1]]
            |= chip8->V_REGS[nibbles[2]];
            break;
        case OP_AND_VX_VY:
            chip8->V_REGS[nibbles[1]]
            &= chip8->V_REGS[nibbles[2]];
            break;
        case OP_XOR_VX_VY:
            chip8->V_REGS[nibbles[1]]
            ^= chip8->V_REGS[nibbles[2]];
            break;
        case OP_ADD_VX_VY:
            temp = chip8->V_REGS[nibbles[1]]
            + chip8->V_REGS[nibbles[2]];
            chip8->V_REGS[15] = temp > 255;
            chip8->V_REGS[nibbles[1]] = (uint8_t)temp;
            break;
        case OP_SUB_VX_VY:
            chip8->V_REGS[15] = chip8->V_REGS[nibbles[1]]
            > chip8->V_REGS[nibbles[2]];
            chip8->V_REGS[nibbles[1]]
            -= chip8->V_REGS[nibbles[2]];
            break;
        case OP_SHR_VX_VY:
            chip8->V_REGS[15] = nibbles[1] & 0x01;
            chip8->V_REGS[nibbles[1]] /= 2;
            break;
        case OP_SUBN_VX_VY:
            chip8->V_REGS[15] = chip8->V_REGS[nibbles[2]]
            > chip8->V_REGS[nibbles[1]];
            chip8->V_REGS[nibbles[1]]
            = chip8->V_REGS[nibbles[2]] - chip8->V_REGS[nibbles[1]];
            break;
        case OP_SHL_VX_VY:
            chip8->V_REGS[15] = nibbles[1] & 0x01;
            chip8->V_REGS[nibbles[1]] *= 2;
            break;
        case OP_SNE_VX_VY:
            if (chip8->V_REGS[nibbles[1]]
            != chip8->V_REGS[nibbles[2]]) chip8->PC += 2;
            break;
        case OP_LD_I_ADDR:
            chip8->I_REG =
            (nibbles[1] << 8)
            + (nibbles[2] << 4)
            + nibbles[3];
            break;
        case OP_JP_V0_ADDR:
            chip8->PC =
            (nibbles[1] << 8)
            + (nibbles[2] << 4)
            + nibbles[3];
            chip8->PC += chip8->V_REGS[0];
            chip8->PC -= 2;
            break;
        case OP_RND_VX_BYTE:
            chip8->RNG = (uint8_t)random_range(0, 255);
            chip8->V_REGS[nibbles[1]]
            = bytes[1] & chip8->RNG;
            break;
        case OP_DRW_VX_VY_NIBBLE:
            for (int i = 0; i < nibbles[3]; i++)
            {
                chip8->DISPLAY[chip8->V_REGS[nibbles[1]] + (chip8->V_REGS[nibbles[2]] * 8)]
                ^= chip8->RAM[chip8->I_REG + i];
            }
            // TODO: implement collision and wrapping!
            break;
        case OP_SKP_VX:
            // TODO
            break;
        case OP_SKNP_VX:
            // TODO
            break;
        case OP_LD_VX_DT:
            chip8->V_REGS[nibbles[1]]
            = chip8->DT;
            break;
        case OP_LD_VX_K:
            // TODO
            break;
        case OP_LD_DT_VX:
            chip8->DT
            = chip8->V_REGS[nibbles[1]];
            break;
        case OP_LD_ST_VX:
            chip8->ST
            = chip8->V_REGS[nibbles[1]];
            break;
        case OP_ADD_I_VX:
            chip8->I_REG
            += chip8->V_REGS[nibbles[1]];
        case OP_LD_F_VX:
            chip8->I_REG
            = chip8->SPRITES[chip8->V_REGS[nibbles[1]]];
            break;
        case OP_LD_B_VX:
            temp = chip8->V_REGS[nibbles[1]];
            chip8->RAM[chip8->I_REG + 2] = temp % 10;
            temp /=10;
            chip8->RAM[chip8->I_REG + 1] = temp % 10;
            temp /=10;
            chip8->RAM[chip8->I_REG] = temp % 10;
            break;
        case OP_LD_I_VX:
            for (int i = 0; i <= nibbles[1]; i++)
            {
                chip8->RAM[chip8->I_REG + i]
                = chip8->V_REGS[i];
            }
            break;
        case OP_LD_VX_I:
            for (int i = 0; i <= nibbles[1]; i++)
            {
                chip8->V_REGS[i]
                = chip8->RAM[chip8->I_REG + i];
            }
            break;
        case OP_SUPER_SCD_NIBBLE:
            // TODO
            break;
        case OP_SUPER_SCR:
            // TODO
            break;
        case OP_SUPER_SCL:
            // TODO
            break;
        case OP_SUPER_EXIT:
            // TODO
            break;
        case OP_SUPER_LOW:
            // TODO
            break;
        case OP_SUPER_HIGH:
            // TODO
            break;
        case OP_SUPER_DRW_VX_VY_0:
            // TODO
            break;
        case OP_SUPER_LD_HF_VX:
            // TODO
            break;
        case OP_SUPER_LD_R_VX:
            // TODO
            break;
        case OP_SUPER_LD_VX_R:
            // TODO
            break;
    }
}
