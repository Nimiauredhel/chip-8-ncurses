#include "instructions.h"

const Instruction_t instructions[] =
{
    { 1, "UNKNOWN (%4x)" }, // 0nnn SYS addr - legacy, ignore
                      //
    // chip 8 instructions
    { 1, "SYS %4x" }, // 0nnn SYS addr - legacy, ignore
    { 0, "CLS" }, // CLS - clear screen
    { 0, "RET" }, // RET - return from subroutine

    { 1, "JP %4x" }, // 1nnn JP addr - set PC to nnn
    { 1, "CALL %4x" }, // 2nnn CALL addr - call subroutine
    { 2, "SE V%u, %d" }, // 3xkk SE Vx, byte - skip next if Vx == kk
    { 2, "SNE V%u, %d" },
    { 2, "SE V%u, V%u" },
    { 2, "LD V%u, %d" },
    { 2, "ADD V%u, %d" }, // 7xkk ADD Vx, byte - set Vx = Vx + kk

    { 2, "LD V%u, V%u" }, // 8xy0 LD Vx, Vy - store value of Vy in Vx
    { 2, "OR V%u, V%u" },
    { 2, "AND V%u, V%u" },
    { 2, "XOR V%u, V%u" },
    { 2, "ADD V%u, V%u" },
    { 2, "SUB V%u, V%u" },
    { 2, "SHR V%u {, V%u}" },
    { 2, "SUBN V%u, V%u" },
    { 2, "SHL V%u {, V%u}" }, // 8xye SHL Vx {, Vy}

    { 2, "SNE V%u, V%u" }, // 9xy0 SNE Vx, Vy

    { 1, "LD I, %4x" }, // Annn LD I, addr
    { 1, "JP V0, %4x" }, // Bnnn JP V0, addr
    { 2, "RND V%u, %d" }, // Cxkk RND Vx, byte
    { 3, "DRW V%u, V%u, %d" }, // Dxyn DRW Vx, Vy, nibble

    { 1, "SKP Vx" }, // Ex9E SKP Vx
    { 1, "SKNP Vx" }, // ExA1 SKNP Vx

    { 1, "LD V%u, DT" }, // Fx07 LD Vx, DT
    { 1, "LD V%u, K" },
    { 1, "LD DT, V%u" },
    { 1, "LD ST, V%u" },
    { 1, "ADD I, V%u" },
    { 1, "LD F, V%u" },
    { 1, "LD B, V%u" },
    { 1, "LD [I], V%u" },
    { 1, "LD V%u, [I]" },

    // super chip 48 instructions
    { 1, "SCD %d" }, // SCD nibble
    { 0, "SCR" },
    { 0, "SCL" },
    { 0, "EXIT" },
    { 0, "LOW" },
    { 0, "HIGH" },

    { 1, "LD HF, V%u" },
    { 1, "LD R, V%u" },
    { 1, "LD V%u, R" },
};
