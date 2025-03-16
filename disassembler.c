#include "disassembler.h"

void disassemble(uint8_t *program_ptr, uint16_t program_size)
{
    uint8_t pc = 0;
    OpcodeIndex_t op_idx = OP_UNKNOWN;
    uint8_t read_bytes[2] = {0};
    uint8_t read_nibbles[4] = {0};

    while (pc < program_size)
    {
        read_bytes[0] = program_ptr[pc];
        read_bytes[1] = program_ptr[pc + 1];

        read_nibbles[0] = read_bytes[0] >> 4;
        read_nibbles[1] = (read_bytes[0] << 4) >> 4;
        read_nibbles[2] = read_bytes[1] >> 4;
        read_nibbles[3] = (read_bytes[1] << 4) >> 4;

        op_idx = parse_instruction(read_bytes, read_nibbles);

        pc += 2;
    }
}

void print_instruction(uint8_t bytes[2], uint8_t nibbles[4], OpcodeIndex_t op_idx)
{
}
