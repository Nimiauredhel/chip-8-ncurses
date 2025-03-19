#include "data.h"

// stored as ints because ncurses
const int chip8_key_table[16] =
{
    CHIP8_KEY_0, CHIP8_KEY_1, CHIP8_KEY_2, CHIP8_KEY_3, CHIP8_KEY_4, CHIP8_KEY_5, CHIP8_KEY_6, CHIP8_KEY_7, 
    CHIP8_KEY_8, CHIP8_KEY_9, CHIP8_KEY_A, CHIP8_KEY_B, CHIP8_KEY_C, CHIP8_KEY_D, CHIP8_KEY_E, CHIP8_KEY_F, 
};

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
    printf("Initializing CHIP-8 instance.");
    Chip8_t *chip8 = malloc(sizeof(Chip8_t));
    explicit_bzero(chip8, sizeof(Chip8_t));
    usleep(100000);

    chip8->emu_state = (EmulatorState_t *)(chip8->RAM + CHIP8_RAM_EMU_STATE_START);
    chip8->display_memory = (chip8->RAM + CHIP8_RAM_DISPLAY_START);
    chip8->registers = (Chip8Registers_t *)(chip8->RAM + CHIP8_RAM_REGISTERS_START);
    chip8->instruction = (Chip8Instruction_t *)(chip8->RAM + CHIP8_RAM_INSTRUCTION_START);

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

