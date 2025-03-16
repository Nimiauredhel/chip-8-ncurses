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
