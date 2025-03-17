#include "data.h"

const uint8_t default_sprites[CHIP8_DEFAULT_SPRITES_SIZE] =
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
    Chip8_t *chip8 = malloc(sizeof(Chip8_t));
    explicit_bzero(chip8, sizeof(Chip8_t));
    printf("Initialized CHIP-8 instance.");
    usleep(200000);

    load_default_sprites(chip8);
    printf("Loaded default sprites.\n");
    usleep(200000);

    load_rom(rom_path, chip8->RAM + CHIP8_DEFAULT_PROGRAM_START);
    chip8->PC = CHIP8_DEFAULT_PROGRAM_START;
    printf("Loaded ROM.\n");
    usleep(200000);

    return chip8;
}

void load_default_sprites(Chip8_t *chip8)
{
    for (uint16_t i = 0; i < CHIP8_DEFAULT_SPRITES_SIZE; i++)
    {
        chip8->RAM[CHIP8_DEFAULT_SPRITES_START + i] = default_sprites[i];
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

