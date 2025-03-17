#include "renderer.h"

void init_display(Chip8_t *chip8, WINDOW **window_chip8, WINDOW **window_emu, WINDOW **window_disassembly)
{
    initscr();

    *window_chip8 = newwin(CHIP8_DISPLAY_HEIGHT+2, CHIP8_DISPLAY_WIDTH+2, 0, 0);
    *window_disassembly = newwin(24, 50, 0, CHIP8_DISPLAY_WIDTH+2);
    *window_emu = newwin(8, 34, 24, CHIP8_DISPLAY_WIDTH+2);
    wclear(*window_chip8);
    wclear(*window_disassembly);
    wclear(*window_emu);
    wrefresh(*window_chip8);
    wrefresh(*window_disassembly);
    wrefresh(*window_emu);

    start_color();
    
    init_pair(1, COLOR_WHITE, COLOR_GREEN); // 1 is on
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // 2 is off

    explicit_bzero(chip8->DISPLAY, 256);
}

void render_display(Chip8_t *chip8, WINDOW *window_chip8)
{
    static const char c[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};

    uint8_t byte;
    uint8_t pixel;
    uint8_t row = 0;
    uint8_t col = 0;

    wclear(window_chip8);
    box(window_chip8, 0, 0);

    for (int idx = 0; idx < CHIP8_DISPLAY_SIZE; idx++)
    {
        byte = chip8->DISPLAY[idx];

        for (int bit = 0; bit < 8; bit++)
        {
            pixel = 2 - ((byte >> (7 - bit)) & 0x01);
            wattron(window_chip8, COLOR_PAIR(pixel));
            mvwaddch(window_chip8, 1 + row, 1 + bit + (col * 8), c[bit]);
            wattroff(window_chip8, COLOR_PAIR(pixel));
        }

        row++;

        if (row > CHIP8_DISPLAY_Y_MAX)
        {
            row = 0;
            col++;
        }
    }

    wrefresh(window_chip8);
}

void render_disassembly(Chip8_t *chip8, WINDOW *window_disassembly)
{
}

void render_emulator_stats(Chip8_t *chip8, WINDOW *window_emu)
{
}
