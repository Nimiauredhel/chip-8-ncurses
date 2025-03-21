#include "data.h"

const uint8_t chip8_default_sprites[CHIP8_DEFAULT_SPRITES_SIZE] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

Chip8_t *create_instance(char *rom_path)
{
    printf("Initializing CHIP-8 instance.\n");
    Chip8_t *chip8 = malloc(sizeof(Chip8_t));
    explicit_bzero(chip8, sizeof(Chip8_t));

    printf("Storing %lu bytes of interpreter data in the lower 512 bytes of CHIP-8 memory.\n",
            sizeof(Chip8Instruction_t) + sizeof(Chip8Registers_t) + sizeof(EmulatorState_t) + 80);
    usleep(100000);

    /*printf("The size of EmulatorState_t is %lu.\n", sizeof(EmulatorState_t));
    printf("The size of Chip8Registers_t is %lu.\n", sizeof(Chip8Registers_t));
    printf("The size of Chip8Instruction_t is %lu.\n", sizeof(Chip8Instruction_t));*/

    chip8->display_memory = (chip8->RAM + CHIP8_RAM_DISPLAY_START);
    chip8->instruction = (Chip8Instruction_t *)(chip8->RAM + CHIP8_RAM_INSTRUCTION_START);
    chip8->registers = (Chip8Registers_t *)(chip8->RAM + CHIP8_RAM_REGISTERS_START);
    chip8->emu_state = (EmulatorState_t *)(chip8->RAM + CHIP8_RAM_EMU_STATE_START);

    load_default_sprites(chip8);
    printf("Loaded default sprites.\n");
    usleep(100000);

    load_rom(rom_path, chip8->RAM + CHIP8_RAM_PROGRAM_START);
    chip8->registers->PC = CHIP8_RAM_PROGRAM_START;
    printf("Loaded program.\n");
    usleep(100000);

    return chip8;
}

void load_default_sprites(Chip8_t *chip8)
{
    for (uint16_t i = 0; i < CHIP8_DEFAULT_SPRITES_SIZE; i++)
    {
        chip8->RAM[CHIP8_RAM_SPRITES_START + i] = chip8_default_sprites[i];
    }
}


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

