#include "common.h"
#include "chip8_defs.h"
#include "data.h"
#include "disassembler.h"
#include "emulator.h"

int main(int argc, char *argv[])
{
    initialize_signal_handler();
    initialize_random_seed();

    printf("Hello!\n");
    usleep(500000);

    if (argc < 2)
    {
        printf("Next time, input a file path. Goodbye!\n");
        sleep(1);
        return EXIT_SUCCESS;
    }

    char *rom_path;

    if (should_terminate)
    {
        printf("Okay, never mind then...");
    }
    else if (strcmp(argv[1], "d") == 0 && argc > 2)
    {
        rom_path = argv[2];
        Chip8_t *chip8 = create_instance(rom_path);
        size_t rom_size = load_rom(rom_path, chip8->RAM + CHIP8_RAM_PROGRAM_START);
        printf("ROM loaded to memory.\n");
        sleep(1);
        disassemble(chip8, CHIP8_RAM_PROGRAM_START + rom_size);
    }
    else
    {
        rom_path = argv[1];
        bool should_run = true;

        while (should_run)
        {
            Chip8_t *chip8 = create_instance(rom_path);
            printf("Beginning execution.\n");
            usleep(200000);
            should_run = run(chip8);
            free(chip8);
        }
    }

    printf("Goodbye.\n");
    usleep(200000);
    return EXIT_SUCCESS;
}
