#include "disassembler.h"

void print_instruction(uint8_t bytes[2], uint8_t nibbles[4], OpcodeIndex_t op_idx)
{
    static uint16_t word_arg = 0;

    switch(instructions[op_idx].schema)
    {
    case OPSCH_NNN:
        word_arg = (nibbles[1] << 8) + (nibbles[2] << 4) + nibbles[3];
        printf(instructions[op_idx].format, word_arg);
        break;
    case OPSCH_XKK:
        printf(instructions[op_idx].format, nibbles[1], bytes[1]);
        break;
    case OPSCH_XY_:
        printf(instructions[op_idx].format, nibbles[1], nibbles[2]);
        break;
    case OPSCH_XYN:
        printf(instructions[op_idx].format, nibbles[1], nibbles[2], nibbles[3]);
        break;
    case OPSCH_X__:
        printf(instructions[op_idx].format, nibbles[1]);
        break;
    case OPSCH___N:
        printf(instructions[op_idx].format, nibbles[3], bytes[1]);
        break;
    case OPSCH_NONE:
    default:
        printf("%s", instructions[op_idx].format);
        break;
    }

    printf(" ~ bytes 0x%02X%02X ~ nibbles 0x%1X%1X%1X%1X \n", bytes[0], bytes[1], nibbles[0], nibbles[1], nibbles[2], nibbles[3]);
}

void printw_instruction(uint8_t bytes[2], uint8_t nibbles[4], OpcodeIndex_t op_idx)
{
    static uint16_t word_arg = 0;

    switch(instructions[op_idx].schema)
    {
    case OPSCH_NNN:
        word_arg = (nibbles[1] << 8) + (nibbles[2] << 4) + nibbles[3];
        printw(instructions[op_idx].format, word_arg);
        break;
    case OPSCH_XKK:
        printw(instructions[op_idx].format, nibbles[1], bytes[1]);
        break;
    case OPSCH_XY_:
        printw(instructions[op_idx].format, nibbles[1], nibbles[2]);
        break;
    case OPSCH_XYN:
        printw(instructions[op_idx].format, nibbles[1], nibbles[2], nibbles[3]);
        break;
    case OPSCH_X__:
        printw(instructions[op_idx].format, nibbles[1]);
        break;
    case OPSCH___N:
        printw(instructions[op_idx].format, nibbles[3], bytes[1]);
        break;
    case OPSCH_NONE:
    default:
        printw("%s", instructions[op_idx].format);
        break;
    }

    printw(" ~ bytes 0x%02X%02X ~ nibbles 0x%1X%1X%1X%1X \n", bytes[0], bytes[1], nibbles[0], nibbles[1], nibbles[2], nibbles[3]);
}

void disassemble(Chip8_t *chip8, uint16_t program_end)
{
    OpcodeIndex_t op_idx = OP_UNKNOWN;
    uint8_t read_bytes[2] = {0};
    uint8_t read_nibbles[4] = {0};

    while (chip8->PC < program_end)
    {
        read_bytes[0] = chip8->RAM[chip8->PC];
        read_bytes[1] = chip8->RAM[chip8->PC + 1];

        read_nibbles[0] = read_bytes[0] >> 4;
        read_nibbles[1] = read_bytes[0] - (read_nibbles[0] << 4);
        read_nibbles[2] = read_bytes[1] >> 4;
        read_nibbles[3] = read_bytes[1] - (read_nibbles[2] << 4);

        op_idx = parse_instruction(read_bytes, read_nibbles);
        printf("[%u] ", chip8->PC);
        print_instruction(read_bytes, read_nibbles, op_idx);

        chip8->PC += 2;
    }
}
