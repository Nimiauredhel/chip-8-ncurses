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

bool run(Chip8_t *chip8)
{
    bool step_mode = false;
    int key = '~';
    uint32_t frame_us = 16000; // 60hz
    OpcodeIndex_t op_idx = OP_UNKNOWN;
    uint8_t read_bytes[2] = {0};
    uint8_t read_nibbles[4] = {0};

    init_display(chip8);
    cbreak();
    nodelay(stdscr, true);

    while (chip8->PC < 4096 && chip8->PC >= 0 && !should_terminate)
    {
        usleep(frame_us);

        key = getch();

        if (step_mode)
        {
            while (key != ' ' && key != 's' && key != 'r' && !should_terminate)
            {
                key = getch();
            }

            usleep(100000);
        }

        switch(key)
        {
            case 'r':
                return true;
            case 's':
                step_mode = !step_mode;
                usleep(100000);
                break;
            case '=':
            case '+':
                if (frame_us > 8000)
                {
                    frame_us /= 2;
                }
                break;
            case '-':
            case '_':
                if (frame_us < 1024000)
                {
                    frame_us *= 2;
                }
                break;
        }

        key = '~';
        
        if (chip8->DT > 0) chip8->DT--;
        if (chip8->ST > 0) chip8->ST--;

        read_bytes[0] = chip8->RAM[chip8->PC];
        read_bytes[1] = chip8->RAM[chip8->PC + 1];

        read_nibbles[0] = read_bytes[0] >> 4;
        read_nibbles[1] = read_bytes[0] - (read_nibbles[0] << 4);
        read_nibbles[2] = read_bytes[1] >> 4;
        read_nibbles[3] = read_bytes[1] - (read_nibbles[2] << 4);

        op_idx = parse_instruction(read_bytes, read_nibbles);
        execute_instruction(chip8, read_bytes, read_nibbles, op_idx);

        clear();

        render_display(chip8);

        mvprintw(0, 64, "[%u] ", chip8->PC);
        printw_instruction(read_bytes, read_nibbles, op_idx);

        for (int i = 0; i < 16; i++)
        {
            mvprintw(1+i, 64, "V%d[%u] ", i, chip8->V_REGS[i]);
        }
        mvprintw(17, 64, "I[%u] ", chip8->I_REG);
        mvprintw(18, 64, "SP[%u] ", chip8->SP);
        mvprintw(19, 64, "DT[%u] ", chip8->DT);
        mvprintw(20, 64, "ST[%u] ", chip8->ST);
        mvprintw(21, 64, "RNG[%u] ", chip8->RNG);

        refresh();

        chip8->PC += 2;
    }

    endwin();
    printf("Program over!\n");
    sleep(1);
    return 0;
}

Chip8_t *create_instance(char *rom_path)
{
    Chip8_t *chip8 = malloc(sizeof(Chip8_t));
    explicit_bzero(chip8, sizeof(Chip8_t));

    load_rom(rom_path, chip8->RAM + PROGRAM_START);
    printf("ROM loaded to memory.\n");

    sleep(1);

    load_sprites(chip8, default_sprites, PROGRAM_START - 80);
    chip8->PC = PROGRAM_START;
    return chip8;
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

    char *rom_path;

    if (strcmp(argv[1], "d") == 0 && argc > 2)
    {
        rom_path = argv[2];
        Chip8_t chip8 = {0};
        size_t rom_size = load_rom(rom_path, chip8.RAM + PROGRAM_START);
        printf("ROM loaded to memory.\n");
        sleep(1);
        chip8.PC = PROGRAM_START;
        disassemble(&chip8, PROGRAM_START + rom_size);
    }
    else
    {
        rom_path = argv[1];
        bool should_run = true;

        while (should_run)
        {
            Chip8_t *chip8 = create_instance(rom_path);
            should_run = run(chip8);
            free(chip8);
        }
    }

    return EXIT_SUCCESS;
}
