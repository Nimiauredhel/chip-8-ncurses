#include "disassembler.h"

void print_instruction(uint8_t bytes[2], uint8_t nibbles[4], OpcodeIndex_t op_idx)
{
    switch(instructions[op_idx].argc)
    {
        case 1:
            printf(instructions[op_idx].format, nibbles[1]);
            break;
        case 2:
            printf(instructions[op_idx].format, nibbles[1], nibbles[2]);
            break;
        case 3:
            printf(instructions[op_idx].format, nibbles[1], nibbles[2], nibbles[3]);
            break;
        case 0:
        default:
            printf("%s", instructions[op_idx].format);
            break;
    }

    printf(" ~ 0x%02X%02X ~ 0x %1X %1X %1X %1X \n", bytes[0], bytes[1], nibbles[0], nibbles[1], nibbles[2], nibbles[3]);
}

void disassemble(uint8_t *program_ptr, size_t program_size)
{
    size_t pc = 0;
    OpcodeIndex_t op_idx = OP_UNKNOWN;
    uint8_t read_bytes[2] = {0};
    uint8_t read_nibbles[4] = {0};

    while (pc < program_size)
    {
        read_bytes[0] = program_ptr[pc];
        read_bytes[1] = program_ptr[pc + 1];

        read_nibbles[0] = read_bytes[0] >> 4;
        read_nibbles[1] = read_bytes[0] - (read_nibbles[0] << 4);
        read_nibbles[2] = read_bytes[1] >> 4;
        read_nibbles[3] = read_bytes[1] - (read_nibbles[2] << 4);

        op_idx = parse_instruction(read_bytes, read_nibbles);
        printf("[%lu] ", pc);
        print_instruction(read_bytes, read_nibbles, op_idx);

        pc += 2;
    }
}
