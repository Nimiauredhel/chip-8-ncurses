#include "disassembler.h"

const char instruction_formats[CHIP8_INSTRUCTION_COUNT][32] =
{
    // any unknown/invalid instructions, which are currently ignored
    "UNKNOWN (%4x)",

    // chip 8 instructions
    "SYS %4x", // 0nnn SYS addr - legacy, ignore
    "CLS", // CLS - clear screen
    "RET", // RET - return from subroutine

    "JP %d", // 1nnn JP addr - set PC to nnn
    "CALL %d", // 2nnn CALL addr - call subroutine
    "SE V%u, %d", // 3xkk SE Vx, byte - skip next if Vx == kk
    "SNE V%u, %d",
    "SE V%u, V%u",
    "LD V%u, %d",
    "ADD V%u, %d", // 7xkk ADD Vx, byte - set Vx = Vx + kk

    "LD V%u, V%u", // 8xy0 LD Vx, Vy - store value of Vy in Vx
    "OR V%u, V%u",
    "AND V%u, V%u",
    "XOR V%u, V%u",
    "ADD V%u, V%u",
    "SUB V%u, V%u",
    "SHR V%u {, V%u}",
    "SUBN V%u, V%u",
    "SHL V%u {, V%u}", // 8xye SHL Vx {, Vy}

    "SNE V%u, V%u", // 9xy0 SNE Vx, Vy

    "LD I, %d", // Annn LD I, addr
    "JP V0, %d", // Bnnn JP V0, addr
    "RND V%u, %d", // Cxkk RND Vx, byte
    "DRW V%u, V%u, %d", // Dxyn DRW Vx, Vy, nibble

    "SKP V%u", // Ex9E SKP Vx
    "SKNP V%u", // ExA1 SKNP Vx

    "LD V%u, DT", // Fx07 LD Vx, DT
    "LD V%u, K",
    "LD DT, V%u",
    "LD ST, V%u",
    "ADD I, V%u",
    "LD F, V%u",
    "LD B, V%u",
    "LD [I], V%u",
    "LD V%u, [I]",

    // super chip 48 instructions
    "SCD %d", // SCD nibble
    "SCR",
    "SCL",
    "EXIT",
    "LOW",
    "HIGH",

    "DRW V%u, V%u, 0",
    "LD HF, V%u",
    "LD R, V%u",
    "LD V%u, R",
};

void print_instruction(Chip8Instruction_t *instruction)
{
    static uint16_t word_arg = 0;

    switch(instruction_schemas[instruction->op_idx])
    {
        case OPSCH_NNN:
            word_arg = PARSE_NNN_TO_U16(instruction->nibbles[1], instruction->nibbles[2], instruction->nibbles[3]);
            printf(instruction_formats[instruction->op_idx], word_arg);
            break;
        case OPSCH_XKK:
            printf(instruction_formats[instruction->op_idx], instruction->nibbles[1], instruction->bytes[1]);
            break;
        case OPSCH_XY_:
            printf(instruction_formats[instruction->op_idx], instruction->nibbles[1], instruction->nibbles[2]);
            break;
        case OPSCH_XYN:
            printf(instruction_formats[instruction->op_idx], instruction->nibbles[1], instruction->nibbles[2], instruction->nibbles[3]);
            break;
        case OPSCH_X__:
            printf(instruction_formats[instruction->op_idx], instruction->nibbles[1]);
            break;
        case OPSCH___N:
            printf(instruction_formats[instruction->op_idx], instruction->nibbles[3], instruction->bytes[1]);
            break;
        case OPSCH_NONE:
        default:
            printf("%s", instruction_formats[instruction->op_idx]);
            break;
    }

    printf(" ~ bytes %u, %u ~ nibbles 0x%1X%1X%1X%1X\n",
    instruction->bytes[0],
    instruction->bytes[1],
    instruction->nibbles[0],
    instruction->nibbles[1],
    instruction->nibbles[2],
    instruction->nibbles[3]);
}

void mvwprintw_instruction(WINDOW *window_disassembly, int row, int col, Chip8Instruction_t *instruction)
{
    static uint16_t word_arg = 0;

    switch(instruction_schemas[instruction->op_idx])
    {
        case OPSCH_NNN:
            word_arg = PARSE_NNN_TO_U16(instruction->nibbles[1],instruction-> nibbles[2], instruction->nibbles[3]);
            mvwprintw(window_disassembly, row, col, instruction_formats[instruction->op_idx], word_arg);
            break;
        case OPSCH_XKK:
            mvwprintw(window_disassembly, row, col, instruction_formats[instruction->op_idx], instruction->nibbles[1], instruction->bytes[1]);
            break;
        case OPSCH_XY_:
            mvwprintw(window_disassembly, row, col, instruction_formats[instruction->op_idx], instruction->nibbles[1], instruction->nibbles[2]);
            break;
        case OPSCH_XYN:
            mvwprintw(window_disassembly, row, col, instruction_formats[instruction->op_idx], instruction->nibbles[1], instruction->nibbles[2], instruction->nibbles[3]);
            break;
        case OPSCH_X__:
            mvwprintw(window_disassembly, row, col, instruction_formats[instruction->op_idx], instruction->nibbles[1]);
            break;
        case OPSCH___N:
            mvwprintw(window_disassembly, row, col, instruction_formats[instruction->op_idx], instruction->nibbles[3], instruction->bytes[1]);
            break;
        case OPSCH_NONE:
        default:
            mvwprintw(window_disassembly, row, col, "%s", instruction_formats[instruction->op_idx]);
            break;
    }

    /*
    mvwprintw(window_disassembly, row, col,
    " ~ bytes %u, %u ~ nibbles 0x%1X%1X%1X%1X",
    instruction->bytes[0],
    instruction->bytes[1],
    instruction->nibbles[0],
    instruction->nibbles[1],
    instruction->nibbles[2],
    instruction->nibbles[3]);
    */
}

void disassemble(Chip8_t *chip8, uint16_t program_end)
{
    Chip8Instruction_t instruction;

    while (chip8->registers->PC < program_end)
    {
        instruction.bytes[0] = chip8->RAM[chip8->registers->PC];
        instruction.bytes[1] = chip8->RAM[chip8->registers->PC + 1];

        instruction.nibbles[0] = instruction.bytes[0] >> 4;
        instruction.nibbles[1] = instruction.bytes[0] - (instruction.nibbles[0] << 4);
        instruction.nibbles[2] = instruction.bytes[1] >> 4;
        instruction.nibbles[3] = instruction.bytes[1] - (instruction.nibbles[2] << 4);

        decode_instruction(&instruction);
        printf("[%u] ", chip8->registers->PC);
        print_instruction(&instruction);

        chip8->registers->PC += 2;
    }
}
