#include "emulator.h"

bool run(Chip8_t *chip8)
{
    const uint32_t tick_delay_us = 8333;
    const uint32_t tick_threshold = EMU_DEFAULT_DELAY_US;
    const struct timespec start_clock;

    bool should_reset = false;
    bool step_mode = false;
    bool step_pressed = false;

    float speed_modifier = 1.0f;
    uint32_t tick_increment = tick_delay_us * speed_modifier;
    uint32_t tick_counter = 0;
    uint32_t frame_counter = 0;
    float seconds_counter = 0.0f;
    float avg_fps;

    int key = '~';
    Instruction_t instruction;
    DisplayLayout_t layout;

    // intentionally discarding the const qualifier ONCE to init the value
    // hope the police don't get me
    clock_gettime(CLOCK_MONOTONIC, (struct timespec *)&start_clock);

    init_display(&layout);
    render_display(chip8, layout.window_chip8);
    cbreak();
    nodelay(stdscr, true);
    noecho();

    while (chip8->PC < 0xFFF && !should_terminate && !should_reset)
    {
        key = getch();
        usleep(tick_delay_us);

        switch(key)
        {
            case 'r':
                should_reset = true;
                break;
            case 's':
                step_mode = true;
                break;
            case '=':
            case '+':
                speed_modifier += EMU_SPEED_INCREMENT;
                if (speed_modifier > EMU_MAX_SPEED_MOD) speed_modifier = EMU_MAX_SPEED_MOD;
                tick_increment = tick_delay_us * speed_modifier;
                break;
            case '-':
            case '_':
                speed_modifier -= EMU_SPEED_INCREMENT;
                if (speed_modifier < EMU_MIN_SPEED_MOD) speed_modifier = EMU_MIN_SPEED_MOD;
                tick_increment = tick_delay_us * speed_modifier;
                break;
            case ' ':
                if (step_mode)
                {
                    step_pressed = true;
                }
                break;
            case '~':
            default:
                break;
        }

        if (should_terminate) break;

        if (step_mode)
        {
            if (key == ' ')
            {
                step_mode = false;
                continue;
            }

            key = '~';

            if (step_pressed)
            {
                step_pressed = false;
            }
            else continue;
        }
        else 
        {
            key = '~';
            seconds_counter = seconds_since_clock(start_clock);

            if(tick_counter < tick_threshold)
            {
                tick_counter += tick_increment;
                continue;
            }
        }

        tick_counter = 0;
        frame_counter++;
        avg_fps = frame_counter / seconds_counter;

        wclear(layout.window_emu);
        box(layout.window_emu, 0, 0);
        mvwprintw(layout.window_emu, 1, 1, "Speed[%.2f]", 1.0f/speed_modifier);
        mvwprintw(layout.window_emu, 2, 1, "Frame Counter[%u]", frame_counter);
        mvwprintw(layout.window_emu, 3, 1, "Runtime[%.2fs]", seconds_counter);
        mvwprintw(layout.window_emu, 4, 1, "Avg. FPS[%.2f]", avg_fps);
        //mvwprintw(window_emu, 5, 1, "Tick Counter[%u/%u]", tick_counter, tick_threshold);
        wrefresh(layout.window_emu);
        
        if (chip8->DT > 0) chip8->DT--;
        if (chip8->ST > 0) chip8->ST--;

        instruction.bytes[0] = chip8->RAM[chip8->PC];
        instruction.bytes[1] = chip8->RAM[chip8->PC + 1];

        instruction.nibbles[0] = instruction.bytes[0] >> 4;
        instruction.nibbles[1] = instruction.bytes[0] - (instruction.nibbles[0] << 4);
        instruction.nibbles[2] = instruction.bytes[1] >> 4;
        instruction.nibbles[3] = instruction.bytes[1] - (instruction.nibbles[2] << 4);

        decode_instruction(&instruction);

        wclear(layout.window_disassembly);
        box(layout.window_disassembly, 0, 0);
        mvwprintw(layout.window_disassembly, 1, 1, "[%u] ", chip8->PC);
        mvwprintw_instruction(layout.window_disassembly, 1, 6, &instruction);

        for (int i = 0; i < 16; i++)
        {
            mvwprintw(layout.window_disassembly, 2+i, 1, "V%d[%u]", i, chip8->V_REGS[i]);
        }

        mvwprintw(layout.window_disassembly, 18, 1, "I[%u]", chip8->I_REG);
        mvwprintw(layout.window_disassembly, 19, 1, "SP[%u]", chip8->SP);
        mvwprintw(layout.window_disassembly, 20, 1, "DT[%u]", chip8->DT);
        mvwprintw(layout.window_disassembly, 21, 1, "ST[%u]", chip8->ST);
        mvwprintw(layout.window_disassembly, 22, 1, "RNG[%u]", chip8->RNG);
        wrefresh(layout.window_disassembly);

        execute_instruction(chip8, &instruction, layout.window_chip8);

        chip8->PC += 2;
    }

    usleep(250000);
    
    delwin(layout.window_chip8);
    delwin(layout.window_disassembly);
    delwin(layout.window_emu);
    endwin();
    printf(chip8->PC > 0xFFF ? "PC out of bounds!\n" : should_reset ? "Restarting!\n" : "Program over!\n");
    usleep(500000);
    return should_reset;
}

void execute_instruction(Chip8_t *chip8, Instruction_t *instruction, WINDOW *window_chip8)
{
    // local macros for the custom TEMP registers I added
#define TEMP0_BYTE0 chip8->EMU_TEMP[0].bytes[0]
#define TEMP0_BYTE1 chip8->EMU_TEMP[0].bytes[1]
#define TEMP1_BYTE0 chip8->EMU_TEMP[1].bytes[0]
#define TEMP1_BYTE1 chip8->EMU_TEMP[1].bytes[1]
#define TEMP2_BYTE0 chip8->EMU_TEMP[2].bytes[0]
#define TEMP2_BYTE1 chip8->EMU_TEMP[2].bytes[1]
#define TEMP0_WORD chip8->EMU_TEMP[0].word
#define TEMP1_WORD chip8->EMU_TEMP[1].word
#define TEMP2_WORD chip8->EMU_TEMP[2].word

    switch (instruction->op_idx)
    {
        case OP_UNKNOWN:
        case OP_SYS_ADDR:
            break;
        case OP_CLS:
            explicit_bzero(chip8->DISPLAY, CHIP8_DISPLAY_SIZE);
            break;
        case OP_RET:
            chip8->PC = chip8->STACK_RET[chip8->SP];
            chip8->PC -= 2;
            chip8->SP--;
            break;
        case OP_CALL_ADDR:
            chip8->SP++;
            chip8->STACK_RET[chip8->SP] = chip8->PC;
            // intentional fall through!
        case OP_JP_ADDR:
            chip8->PC = PARSE_NNN_TO_U16
            (instruction->nibbles[1],
             instruction->nibbles[2],
             instruction->nibbles[3]);
            chip8->PC -= 2;
            break;
        case OP_SE_VX_BYTE:
            if (chip8->V_REGS[instruction->nibbles[1]]
            == instruction->bytes[1]) chip8->PC += 2;
            break;
        case OP_SNE_VX_BYTE:
            if (chip8->V_REGS[instruction->nibbles[1]]
            != instruction->bytes[1]) chip8->PC += 2;
            break;
        case OP_SE_VX_VY:
            if (chip8->V_REGS[instruction->nibbles[1]]
            == chip8->V_REGS[instruction->nibbles[2]]) chip8->PC += 2;
            break;
        case OP_LD_VX_BYTE:
            chip8->V_REGS[instruction->nibbles[1]]
            = instruction->bytes[1];
            break;
        case OP_ADD_VX_BYTE:
            chip8->V_REGS[instruction->nibbles[1]]
            += instruction->bytes[1];
            break;
        case OP_LD_VX_VY:
            chip8->V_REGS[instruction->nibbles[1]]
            = chip8->V_REGS[instruction->nibbles[2]];
            break;
        case OP_OR_VX_VY:
            chip8->V_REGS[instruction->nibbles[1]]
            |= chip8->V_REGS[instruction->nibbles[2]];
            break;
        case OP_AND_VX_VY:
            chip8->V_REGS[instruction->nibbles[1]]
            &= chip8->V_REGS[instruction->nibbles[2]];
            break;
        case OP_XOR_VX_VY:
            chip8->V_REGS[instruction->nibbles[1]]
            ^= chip8->V_REGS[instruction->nibbles[2]];
            break;
        case OP_ADD_VX_VY:
            chip8->EMU_TEMP[0].word = chip8->V_REGS[instruction->nibbles[1]]
            + chip8->V_REGS[instruction->nibbles[2]];
            chip8->V_REGS[15] = chip8->EMU_TEMP[0].word > 255; // set VF to 1 if result greater than 255.
            chip8->V_REGS[instruction->nibbles[1]] // setting Vx to the result AND 255,
            = (uint8_t)(chip8->EMU_TEMP[0].word & 255); // to properly discard bits 8 and up
            break;
        case OP_SUB_VX_VY:
            chip8->V_REGS[15] = chip8->V_REGS[instruction->nibbles[1]]
            > chip8->V_REGS[instruction->nibbles[2]];
            chip8->V_REGS[instruction->nibbles[1]]
            -= chip8->V_REGS[instruction->nibbles[2]];
            break;
        case OP_SHR_VX_VY:
            chip8->V_REGS[15] = instruction->nibbles[1] & 0x01;
            chip8->V_REGS[instruction->nibbles[1]] /= 2;
            break;
        case OP_SUBN_VX_VY:
            chip8->V_REGS[15] = chip8->V_REGS[instruction->nibbles[2]]
            > chip8->V_REGS[instruction->nibbles[1]];
            chip8->V_REGS[instruction->nibbles[1]]
            = chip8->V_REGS[instruction->nibbles[2]] - chip8->V_REGS[instruction->nibbles[1]];
            break;
        case OP_SHL_VX_VY:
            chip8->V_REGS[15] = instruction->nibbles[1] & 0x01;
            chip8->V_REGS[instruction->nibbles[1]] *= 2;
            break;
        case OP_SNE_VX_VY:
            if (chip8->V_REGS[instruction->nibbles[1]]
            != chip8->V_REGS[instruction->nibbles[2]]) chip8->PC += 2;
            break;
        case OP_LD_I_ADDR:
            chip8->I_REG = PARSE_NNN_TO_U16
            (instruction->nibbles[1],
            instruction->nibbles[2],
            instruction->nibbles[3]);
            break;
        case OP_JP_V0_ADDR:
            chip8->PC = PARSE_NNN_TO_U16
            (instruction->nibbles[1],
             instruction->nibbles[2],
             instruction->nibbles[3]);
            chip8->PC += chip8->V_REGS[0];
            chip8->PC -= 2;
            break;
        case OP_RND_VX_BYTE:
            chip8->RNG = (uint8_t)random_range(0, 255);
            chip8->V_REGS[instruction->nibbles[1]]
            = instruction->bytes[1] & chip8->RNG;
            break;
        case OP_DRW_VX_VY_NIBBLE:
#define X TEMP0_BYTE0
#define Y TEMP0_BYTE1
#define READ_POSITION TEMP1_BYTE0
#define READ_END TEMP1_BYTE1
#define WRITE_POSITION TEMP2_BYTE0
#define BIT_OFFSET TEMP2_BYTE1
            // VF = collision, init to zero, actual check later
            chip8->V_REGS[15] = 0;

            X = chip8->V_REGS[instruction->nibbles[1]]
            & CHIP8_DISPLAY_X_MAX; // set x coord (AND 63 for wrapping)
            Y = chip8->V_REGS[instruction->nibbles[2]]
            & CHIP8_DISPLAY_Y_MAX; // set y coord (AND 31 for wrapping)

            // iterating up to <last nibble> rows, reading from <I register> RAM location,
            // writing to display starting from <x, y> and going down
            READ_POSITION = chip8->I_REG;
            READ_END = chip8->I_REG + instruction->nibbles[3];

            for (; READ_POSITION < READ_END; READ_POSITION++)
            {
                // translating coords to array index for this iteration
                WRITE_POSITION = CHIP8_DISPLAY_INDEX(X, Y);

                // check offset from 8-bit alignment
                BIT_OFFSET = X % 8;

                // check for collision (if not collided yet)
                chip8->V_REGS[15] |= (chip8->DISPLAY[WRITE_POSITION]
                                   & (chip8->RAM[READ_POSITION] >> BIT_OFFSET));
                chip8->V_REGS[15] |= (chip8->DISPLAY[(WRITE_POSITION+1) & CHIP8_DISPLAY_SIZE]
                                   & (chip8->RAM[READ_POSITION] << (8 - BIT_OFFSET)));
                // TODO: figure out if this works as well as or better than the following:
                // if (chip8->V_REGS[15] == 0) chip8->V_REGS[15]
                // = chip8->DISPLAY[TEMP1_BYTE1] & chip8->RAM[TEMP1_BYTE0];

                // actually XORing the pixels together
                chip8->DISPLAY[WRITE_POSITION]
                ^= (chip8->RAM[READ_POSITION] >> BIT_OFFSET);
                chip8->DISPLAY[(WRITE_POSITION+1) & CHIP8_DISPLAY_SIZE]
                ^= (chip8->RAM[READ_POSITION] << (8 - BIT_OFFSET));

                // incrementing x coord for next iteration
                X++;

                if (X > CHIP8_DISPLAY_X_MAX)
                {
                    // incrementing y when x wraps around
                    X &= CHIP8_DISPLAY_X_MAX;
                    Y = (Y + 1) & CHIP8_DISPLAY_Y_MAX;
                }
            }

            render_display(chip8, window_chip8);
            break;
        case OP_SKP_VX:
            // TODO
            break;
        case OP_SKNP_VX:
            // TODO
            break;
        case OP_LD_VX_DT:
            chip8->V_REGS[instruction->nibbles[1]]
            = chip8->DT;
            break;
        case OP_LD_VX_K:
            // TODO
            break;
        case OP_LD_DT_VX:
            chip8->DT
            = chip8->V_REGS[instruction->nibbles[1]];
            break;
        case OP_LD_ST_VX:
            chip8->ST
            = chip8->V_REGS[instruction->nibbles[1]];
            break;
        case OP_ADD_I_VX:
            chip8->I_REG
            += chip8->V_REGS[instruction->nibbles[1]];
            break;
        case OP_LD_F_VX:
            // Setting I register to RAM address of <Vx> default sprite
            chip8->I_REG
            = CHIP8_DEFAULT_SPRITES_START
            + (CHIP8_DEFAULT_SPRITE_HEIGHT
            * chip8->V_REGS[instruction->nibbles[1]]);
            break;
        case OP_LD_B_VX:
            // Setting the three bytes starting at RAM <I register> address
            // to the three digits of the Vx integer.
            // TODO: figure out if this is the best way to do this.
            // It's not consequential for CHIP-8, but good to know.
            TEMP0_BYTE0 = chip8->V_REGS[instruction->nibbles[1]];
            TEMP0_BYTE1 = chip8->V_REGS[instruction->nibbles[1]] /= 10;
            TEMP1_BYTE0 = chip8->V_REGS[instruction->nibbles[1]] /= 100;
            chip8->RAM[chip8->I_REG + 2] = TEMP0_BYTE0 % 10;
            chip8->RAM[chip8->I_REG + 1] = TEMP0_BYTE1 % 10;
            chip8->RAM[chip8->I_REG] = TEMP1_BYTE0 % 10;
            break;
        case OP_LD_I_VX:
            for (int i = 0; i <= instruction->nibbles[1]; i++)
            {
                chip8->RAM[chip8->I_REG + i]
                = chip8->V_REGS[i];
            }
            break;
        case OP_LD_VX_I:
            for (int i = 0; i <= instruction->nibbles[1]; i++)
            {
                chip8->V_REGS[i]
                = chip8->RAM[chip8->I_REG + i];
            }
            break;
        case OP_SUPER_SCD_NIBBLE:
            // TODO
            break;
        case OP_SUPER_SCR:
            // TODO
            break;
        case OP_SUPER_SCL:
            // TODO
            break;
        case OP_SUPER_EXIT:
            // TODO
            break;
        case OP_SUPER_LOW:
            // TODO
            break;
        case OP_SUPER_HIGH:
            // TODO
            break;
        case OP_SUPER_DRW_VX_VY_0:
            // TODO
            break;
        case OP_SUPER_LD_HF_VX:
            // TODO
            break;
        case OP_SUPER_LD_R_VX:
            // TODO
            break;
        case OP_SUPER_LD_VX_R:
            // TODO
            break;
        default:
            break;
    }
    // undefining the macros to keep them local
#undef TEMP0_BYTE0
#undef TEMP0_BYTE1
#undef TEMP1_BYTE0
#undef TEMP1_BYTE1
#undef TEMP0_WORD 
#undef TEMP1_WORD 
}
