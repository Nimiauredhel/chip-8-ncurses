#include "common.h"
#include "disassembler.h"
#include "data.h"

#define PROGRAM_START 0x200

void init_display(Chip8_t *chip8)
{
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN); // 1 is on
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // 2 is off
    explicit_bzero(chip8->DISPLAY, 256);
}

void render_display(Chip8_t *chip8)
{
    //char c[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};
    uint8_t byte;
    uint8_t pixel;

    clear();

    for (int row = 0; row < 32; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            byte = chip8->DISPLAY[col + (row * 8)];

            for (int bit = 0; bit < 8; bit++)
            {
                pixel = 2 - ((byte >> bit) & 0x01);
                attron(COLOR_PAIR(pixel));
                mvaddch(row, bit + (col * 8), ' ');//c[bit]);
                attroff(COLOR_PAIR(pixel));
            }
        }
    }

    refresh();
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

void run(Chip8_t *chip8)
{
    OpcodeIndex_t op_idx = OP_UNKNOWN;
    uint8_t read_bytes[2] = {0};
    uint8_t read_nibbles[4] = {0};

    init_display(chip8);

    while (chip8->PC < 4096 && chip8->PC >= 0 && !should_terminate)
    {
        usleep(16000); // 60hz
        
        if (chip8->DT > 0) chip8->DT--;
        if (chip8->ST > 0) chip8->ST--;

        read_bytes[0] = chip8->RAM[chip8->PC];
        read_bytes[1] = chip8->RAM[chip8->PC + 1];

        read_nibbles[0] = read_bytes[0] >> 4;
        read_nibbles[1] = read_bytes[0] - (read_nibbles[0] << 4);
        read_nibbles[2] = read_bytes[1] >> 4;
        read_nibbles[3] = read_bytes[1] - (read_nibbles[2] << 4);

        op_idx = parse_instruction(read_bytes, read_nibbles);
        printf("[%u] ", chip8->PC);
        print_instruction(read_bytes, read_nibbles, op_idx);
        execute_instruction(chip8, read_bytes, read_nibbles, op_idx);
        render_display(chip8);

        chip8->PC += 2;
    }

    endwin();
    printf("Program over!\n");
    sleep(1);
}

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

    Chip8_t chip8 = {0};

    char *rom_path = argv[1];

    //size_t rom_size = load_rom(rom_path, chip8.RAM + PROGRAM_START);
    load_rom(rom_path, chip8.RAM + PROGRAM_START);
    printf("ROM loaded to memory.\n");

    sleep(1);

    load_sprites(&chip8, default_sprites, PROGRAM_START - 80);
    chip8.PC = PROGRAM_START;
    run(&chip8);
    //disassemble(&chip8, PROGRAM_START + rom_size);

    return EXIT_SUCCESS;
}
