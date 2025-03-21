#include "renderer.h"

void init_display(DisplayLayout_t *layout)
{
    initscr();

    nodelay(stdscr, true);
    noecho();
    cbreak();

    layout->window_chip8 = newwin(CHIP8_DISPLAY_HEIGHT, CHIP8_DISPLAY_WIDTH, 0, 0);
    layout->window_disassembly = newwin(1, 64, 2, CHIP8_DISPLAY_WIDTH);
    layout->window_registers = newwin(18, 64, 5, CHIP8_DISPLAY_WIDTH);
    layout->window_emu = newwin(8, 64, CHIP8_DISPLAY_HEIGHT - 8, CHIP8_DISPLAY_WIDTH);

    start_color();
    
    init_pair(1, COLOR_WHITE, COLOR_GREEN); // 1 is on
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // 2 is off
}

void render_display(Chip8_t *chip8, WINDOW *window_chip8)
{
 //   static const char c[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};

    uint8_t byte;
    uint8_t pixel;
    uint8_t row = 0;
    uint8_t col = 0;

    //werase(window_chip8);
    //box(window_chip8, 0, 0);

    for (int idx = 0; idx < CHIP8_DISPLAY_BYTES; idx++)
    {
        col = idx / CHIP8_DISPLAY_HEIGHT;
        row = idx % CHIP8_DISPLAY_HEIGHT;
        byte = chip8->display_memory[idx];

        for (int bit = 0; bit < 8; bit++)
        {
            pixel = 2 - ((byte >> (7 - bit)) & 0x01);
            wattron(window_chip8, COLOR_PAIR(pixel));
            mvwaddch(window_chip8, row, bit + (col * 8), ' '); //c[bit]);
            wattroff(window_chip8, COLOR_PAIR(pixel));
        }
    }

    wrefresh(window_chip8);
}

void render_disassembly(Chip8Instruction_t *instruction, WINDOW *window_disassembly)
{
    werase(window_disassembly);

    mvwprintw_instruction(window_disassembly, 0, 1, instruction);

    wrefresh(window_disassembly);
}

void render_registers(Chip8Registers_t *registers, WINDOW *window_registers)
{
    werase(window_registers);
    box(window_registers, 0, 0);

    for (int i = 0; i < 16; i++)
    {
        mvwprintw(window_registers, i+1, 1, "V%d[%u]", i, registers->V_REGS[i]);
        mvwprintw(window_registers, i+1, 49, "STACK_RET%d[%u]", i, registers->STACK_RET[i]);
    }

    mvwprintw(window_registers, 1, 9, "PC[%u] ", registers->PC);
    mvwprintw(window_registers, 2, 9, "SP[%u]", registers->SP);
    mvwprintw(window_registers, 3, 9, "I[%u]", registers->I_REG);
    mvwprintw(window_registers, 4, 9, "DT[%u]", registers->DT);
    mvwprintw(window_registers, 5, 9, "ST[%u]", registers->ST);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            mvwprintw(window_registers, 1 + (i * 3) + j, 17, "TEMP%u_BYTE%u[%u] ", i, j, registers->EMU_TEMP[i].bytes[j]);
        }

        mvwprintw(window_registers, 1 + (i * 3) + 2, 17, "TEMP%u_WORD[%u] ", i, registers->EMU_TEMP[i].word);
    }

    wrefresh(window_registers);
}

void render_emulator_state(EmulatorState_t *emu_state, WINDOW *window_emu)
{
    werase(window_emu);
    box(window_emu, 0, 0);
    mvwprintw(window_emu, 1, 1, "Speed[%.2f]", emu_state->speed_modifier);
    mvwprintw(window_emu, 2, 1, "Step Counter[%u]", emu_state->step_counter);
    mvwprintw(window_emu, 3, 1, "Runtime[%.2fs]", emu_state->runtime_seconds_counter);
    mvwprintw(window_emu, 4, 1, "Avg. FPS[%.2f]", emu_state->avg_cps);

    for (int i = 0; i < CHIP8_KEY_COUNT; i++)
    {
        mvwprintw(window_emu, (i/4)+1, (8*(i%4))+24, "%s[%u] ", chip8_key_names[i], emu_state->chip8_key_states[i]);
    }

    //mvwprintw(window_emu, 5, 1, "Tick Counter[%u/%u]", tick_counter, tick_threshold);
    wrefresh(window_emu);
}
