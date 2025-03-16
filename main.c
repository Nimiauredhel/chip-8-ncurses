#include "common.h"
#include "disassembler.h"
#include "data.h"

#define PROGRAM_START 0x200

size_t load_rom(char *rom_path, uint8_t *dest_ptr)
{
    FILE *file;
    size_t file_size;

    printf("Loading ROM at path: %s\n", rom_path);

    file = fopen(rom_path, "rb");

    if (file == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_SUCCESS);
    }

    printf("ROM opened successfully.\n");

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);

    printf("ROM size: %lu bytes.\n", file_size);

    rewind(file);
    
    fread(dest_ptr, 1, file_size, file);
    fclose(file);

    return file_size;
}

void run(Chip8_t *chip8)
{
    OpcodeIndex_t op_idx = OP_UNKNOWN;
    uint8_t read_bytes[2] = {0};
    uint8_t read_nibbles[4] = {0};

    while (!chip8->quit)
    {
        read_bytes[0] = chip8->RAM[chip8->PC];
        read_bytes[1] = chip8->RAM[chip8->PC + 1];

        read_nibbles[0] = read_bytes[0] >> 4;
        read_nibbles[1] = read_bytes[0] - (read_nibbles[0] << 4);
        read_nibbles[2] = read_bytes[1] >> 4;
        read_nibbles[3] = read_bytes[1] - (read_nibbles[2] << 4);

        op_idx = parse_instruction(read_bytes, read_nibbles);
        execute_instruction(chip8, read_bytes, read_nibbles, op_idx);

        chip8->PC += 2;
    }
}

int main(int argc, char *argv[])
{
    printf("Hello!\n");
    usleep(500000);

    if (argc < 2)
    {
        printf("Next time, input a file path. Goodbye!\n");
        sleep(1);
        return EXIT_SUCCESS;
    }

    Chip8_t chip8 = {0};

    char *rom_path = argv[1];

    size_t rom_size = load_rom(rom_path, chip8.RAM + PROGRAM_START);
    printf("ROM loaded to memory.\n");

    sleep(1);

    chip8.PC = PROGRAM_START;
    //run(&chip8);
    disassemble(&chip8, PROGRAM_START + rom_size);

    return EXIT_SUCCESS;
}
