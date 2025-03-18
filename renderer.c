#include "renderer.h"

void init_display(DisplayLayout_t *layout)
{
    initscr();

    layout->window_chip8 = newwin(CHIP8_DISPLAY_HEIGHT+2, CHIP8_DISPLAY_WIDTH+2, 0, 0);
    layout->window_disassembly = newwin(4, 64, 0, CHIP8_DISPLAY_WIDTH+2);
    layout->window_registers = newwin(24, 64, 4, CHIP8_DISPLAY_WIDTH+2);
    layout->window_emu = newwin(8, 32, 28, CHIP8_DISPLAY_WIDTH+2);
    wclear(layout->window_chip8);
    wclear(layout->window_disassembly);
    wclear(layout->window_emu);
    wrefresh(layout->window_chip8);
    wrefresh(layout->window_disassembly);
    wrefresh(layout->window_emu);

    start_color();
    
    init_pair(1, COLOR_WHITE, COLOR_GREEN); // 1 is on
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // 2 is off
}

void render_display(Chip8_t *chip8, WINDOW *window_chip8)
{
    static const char c[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};

    uint8_t byte;
    uint8_t pixel;
    uint8_t row = 0;
    uint8_t col = 0;

    //wclear(window_chip8);
    //box(window_chip8, 0, 0);

    for (int idx = 0; idx < CHIP8_DISPLAY_BYTES; idx++)
    {
        col = idx % CHIP8_DISPLAY_ROW_BYTES;
        row = idx / CHIP8_DISPLAY_ROW_BYTES;
        byte = chip8->display_memory[idx];

        for (int bit = 0; bit < 8; bit++)
        {
            pixel = 2 - ((byte >> (7 - bit)) & 0x01);
            wattron(window_chip8, COLOR_PAIR(pixel));
            mvwaddch(window_chip8, 1 + row, 1 + bit + (col * 8), c[bit]);
            wattroff(window_chip8, COLOR_PAIR(pixel));
        }
    }

    wrefresh(window_chip8);
}

void render_disassembly(Chip8Instruction_t *instruction, WINDOW *window_disassembly)
{
    wclear(window_disassembly);
    box(window_disassembly, 0, 0);

    mvwprintw_instruction(window_disassembly, 1, 6, instruction);

    wrefresh(window_disassembly);
}

void render_registers(Chip8Registers_t *registers, WINDOW *window_registers)
{
    wclear(window_registers);
    box(window_registers, 0, 0);

    mvwprintw(window_registers, 1, 1, "PC[%u] ", registers->PC);

    for (int i = 2; i <= 18; i++)
    {
        mvwprintw(window_registers, i, 1, "V%d[%u]", i, registers->V_REGS[i]);
    }

    mvwprintw(window_registers, 19, 1, "I[%u]", registers->I_REG);
    mvwprintw(window_registers, 20, 1, "SP[%u]", registers->SP);
    mvwprintw(window_registers, 21, 1, "DT[%u]", registers->DT);
    mvwprintw(window_registers, 22, 1, "ST[%u]", registers->ST);

    wrefresh(window_registers);
}

void render_emulator_state(EmulatorState_t *emu_state, WINDOW *window_emu)
{
    wclear(window_emu);
    box(window_emu, 0, 0);
    mvwprintw(window_emu, 1, 1, "Speed[%.2f]", emu_state->speed_modifier);
    mvwprintw(window_emu, 2, 1, "Frame Counter[%u]", emu_state->frame_counter);
    mvwprintw(window_emu, 3, 1, "Runtime[%.2fs]", emu_state->seconds_counter);
    mvwprintw(window_emu, 4, 1, "Avg. FPS[%.2f]", emu_state->avg_fps);
    //mvwprintw(window_emu, 5, 1, "Tick Counter[%u/%u]", tick_counter, tick_threshold);
    wrefresh(window_emu);
}
