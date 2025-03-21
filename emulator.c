#include "emulator.h"

void emu_handle_input(Chip8_t *chip8)
{
    if (!check_input(chip8->emu_state->emu_key_states, EMU_KEY_ANY_IDX)) return;

    if (check_input(chip8->emu_state->emu_key_states, EMU_KEY_QUIT_IDX))
        should_terminate = true;

    if (check_input(chip8->emu_state->emu_key_states, EMU_KEY_RESET_IDX))
        chip8->emu_state->should_reset = true;

    if (check_input(chip8->emu_state->emu_key_states, EMU_KEY_STEP_MODE_IDX))
        chip8->emu_state->step_mode = !chip8->emu_state->step_mode;

    if (check_input(chip8->emu_state->emu_key_states, EMU_KEY_STEP_ONE_IDX))
        chip8->emu_state->step_pressed = true;

    if (check_input(chip8->emu_state->emu_key_states, EMU_KEY_SPEED_UP_IDX))
    {
        chip8->emu_state->speed_modifier += EMU_SPEED_INCREMENT;
        if (chip8->emu_state->speed_modifier > EMU_MAX_SPEED_MOD) chip8->emu_state->speed_modifier = EMU_MAX_SPEED_MOD;
        chip8->emu_state->ideal_step_delay_us = EMU_DEFAULT_STEP_DELAY_US / chip8->emu_state->speed_modifier;
    }
    else if (check_input(chip8->emu_state->emu_key_states, EMU_KEY_SPEED_DOWN_IDX))
    {
        chip8->emu_state->speed_modifier -= EMU_SPEED_INCREMENT;
        if (chip8->emu_state->speed_modifier < EMU_MIN_SPEED_MOD) chip8->emu_state->speed_modifier = EMU_MIN_SPEED_MOD;
        chip8->emu_state->ideal_step_delay_us = EMU_DEFAULT_STEP_DELAY_US / chip8->emu_state->speed_modifier;
    }
}

bool run(Chip8_t *chip8)
{
    chip8->emu_state->should_reset = false;
    chip8->emu_state->step_mode = false;
    chip8->emu_state->step_pressed = false;
    chip8->emu_state->loop = false;

    chip8->emu_state->speed_modifier = 1.0f;
    chip8->emu_state->ideal_step_delay_us = EMU_DEFAULT_STEP_DELAY_US;
    chip8->emu_state->step_counter = 0;
    chip8->emu_state->runtime_seconds_counter = 0.0f;
    chip8->emu_state->cycle_seconds_counter = 0.0f;

    init_display(&chip8->layout);
    render_display(chip8, chip8->layout.window_chip8);
    render_registers(chip8->registers, chip8->layout.window_registers);
    render_disassembly(chip8->instruction, chip8->layout.window_disassembly);
    render_emulator_state(chip8->emu_state, chip8->layout.window_emu);

    usleep(chip8->emu_state->ideal_step_delay_us);


    while (chip8->registers->PC < 0xFFF && !should_terminate && !chip8->emu_state->should_reset)
    {
        // getting the start clock of the cycle to approximate our ideal frequency
        clock_gettime(CLOCK_MONOTONIC, (struct timespec *)&chip8->emu_state->start_clock);

        // render emulator (mostly timing) stats
        // TODO: as noted above, make this optional
        render_emulator_state(chip8->emu_state, chip8->layout.window_emu);

        // render content of Chip-8 & extra VM registers.
        // TODO: make this optional
        render_registers(chip8->registers, chip8->layout.window_registers);

        read_input(chip8->emu_state->emu_key_states, chip8->emu_state->chip8_key_states);
        emu_handle_input(chip8);

        if (should_terminate) break;

        if (chip8->emu_state->step_mode
        && !check_input(chip8->emu_state->emu_key_states, EMU_KEY_STEP_ONE_IDX))
        {
            continue;
        }

        chip8->emu_state->step_counter++;

        // parsing current instruction elements into a structure
        chip8->instruction->bytes[0] = chip8->RAM[chip8->registers->PC];
        chip8->instruction->bytes[1] = chip8->RAM[chip8->registers->PC + 1];

        chip8->instruction->nibble0 = chip8->instruction->bytes[0] >> 4;
        chip8->instruction->nibble1 = chip8->instruction->bytes[0] - (chip8->instruction->nibble0 << 4);
        chip8->instruction->nibble2 = chip8->instruction->bytes[1] >> 4;
        chip8->instruction->nibble3 = chip8->instruction->bytes[1] - (chip8->instruction->nibble2 << 4);

        // decoding & updating the opcode field in the instruction struct
        decode_instruction(chip8->instruction);

        // rendering the disassembled instruction
        // TODO: make this optional
        render_disassembly(chip8->instruction, chip8->layout.window_disassembly);

        // executing the actual instruction;
        // this also refreshes the display if necessary
        // TODO: figure out if setting a display_dirty flag would be better
        execute_instruction(chip8, chip8->instruction, chip8->layout.window_chip8);

        if (!chip8->emu_state->loop)
        {
            // default PC incrementation following execution;
            // jump/skip instructions account for this by decrementing the PC to compensate.
            // TODO: figure out if incrementing in the instruction code could be an improvement
            chip8->registers->PC += 2;
            // deincrementing timer registers
            if (chip8->registers->DT > 0) chip8->registers->DT--;
            if (chip8->registers->ST > 0) chip8->registers->ST--;
        }

        // updating timing stats
        chip8->emu_state->cycle_seconds_counter = seconds_since_clock(&chip8->emu_state->start_clock);

        chip8->emu_state->difference_step_delay_us = chip8->emu_state->ideal_step_delay_us - (1000000 * chip8->emu_state->cycle_seconds_counter);

        if (chip8->emu_state->difference_step_delay_us > 0)
        {
            usleep(chip8->emu_state->difference_step_delay_us);
        }

        chip8->emu_state->cycle_seconds_counter = seconds_since_clock(&chip8->emu_state->start_clock);
        chip8->emu_state->runtime_seconds_counter += chip8->emu_state->cycle_seconds_counter;
        chip8->emu_state->avg_cps = chip8->emu_state->avg_cps == 0.0f ? 1.0f / chip8->emu_state->cycle_seconds_counter
        : ((1.0f / chip8->emu_state->cycle_seconds_counter) + chip8->emu_state->avg_cps) / 2.0f;
    }

    usleep(250000);
    
    delwin(chip8->layout.window_chip8);
    delwin(chip8->layout.window_disassembly);
    delwin(chip8->layout.window_registers);
    delwin(chip8->layout.window_emu);
    endwin();
    printf(chip8->registers->PC >= 0xFFF ? "PC out of bounds!\n" : chip8->emu_state->should_reset ? "Restarting!\n" : "Program over!\n");
    usleep(500000);

    // return the "reset" flag, telling the program whether to quit or create another Chip-8 instance
    return chip8->emu_state->should_reset;
}

void execute_instruction(Chip8_t *chip8, Chip8Instruction_t *instruction, WINDOW *window_chip8)
{
    // local macros for registers
#define Vx chip8->registers->V_REGS[instruction->nibble1]
#define Vy chip8->registers->V_REGS[instruction->nibble2]
#define VF chip8->registers->V_REGS[15]
#define TEMP0_BYTE0 chip8->registers->EMU_TEMP[0].bytes[0]
#define TEMP0_BYTE1 chip8->registers->EMU_TEMP[0].bytes[1]
#define TEMP1_BYTE0 chip8->registers->EMU_TEMP[1].bytes[0]
#define TEMP1_BYTE1 chip8->registers->EMU_TEMP[1].bytes[1]
#define TEMP2_BYTE0 chip8->registers->EMU_TEMP[2].bytes[0]
#define TEMP2_BYTE1 chip8->registers->EMU_TEMP[2].bytes[1]
#define TEMP0_WORD chip8->registers->EMU_TEMP[0].word
#define TEMP1_WORD chip8->registers->EMU_TEMP[1].word
#define TEMP2_WORD chip8->registers->EMU_TEMP[2].word

    switch (instruction->op_idx)
    {
        case OP_UNKNOWN:
        case OP_SYS_ADDR:
            break;
        case OP_CLS:
            explicit_bzero(chip8->display_memory, CHIP8_DISPLAY_BYTES);
            render_display(chip8, window_chip8);
            break;
        case OP_RET:
            chip8->registers->PC = chip8->registers->STACK_RET[chip8->registers->SP];
            chip8->registers->STACK_RET[chip8->registers->SP] = 0;
            chip8->registers->SP--;
            break;
        case OP_CALL_ADDR:
            chip8->registers->SP++;
            chip8->registers->STACK_RET[chip8->registers->SP] = chip8->registers->PC;
            // intentional fall through!
        case OP_JP_ADDR:
            chip8->registers->PC = PARSE_NNN_TO_U16
            (instruction->nibble1,
             instruction->nibble2,
             instruction->nibble3);
            chip8->registers->PC -= 2;
            break;
        case OP_SE_VX_BYTE:
            if (chip8->registers->V_REGS[instruction->nibble1]
            == instruction->bytes[1]) chip8->registers->PC += 2;
            break;
        case OP_SNE_VX_BYTE:
            if (chip8->registers->V_REGS[instruction->nibble1]
            != instruction->bytes[1]) chip8->registers->PC += 2;
            break;
        case OP_SE_VX_VY:
            if (chip8->registers->V_REGS[instruction->nibble1]
            == chip8->registers->V_REGS[instruction->nibble2]) chip8->registers->PC += 2;
            break;
        case OP_LD_VX_BYTE:
            chip8->registers->V_REGS[instruction->nibble1]
            = instruction->bytes[1];
            break;
        case OP_ADD_VX_BYTE:
            chip8->registers->V_REGS[instruction->nibble1]
            += instruction->bytes[1];
            break;
        case OP_LD_VX_VY:
            chip8->registers->V_REGS[instruction->nibble1]
            = chip8->registers->V_REGS[instruction->nibble2];
            break;
        case OP_OR_VX_VY:
            chip8->registers->V_REGS[instruction->nibble1]
            |= chip8->registers->V_REGS[instruction->nibble2];
            break;
        case OP_AND_VX_VY:
            chip8->registers->V_REGS[instruction->nibble1]
            &= chip8->registers->V_REGS[instruction->nibble2];
            break;
        case OP_XOR_VX_VY:
            chip8->registers->V_REGS[instruction->nibble1]
            ^= chip8->registers->V_REGS[instruction->nibble2];
            break;
        case OP_ADD_VX_VY:
            chip8->registers->EMU_TEMP[0].word = chip8->registers->V_REGS[instruction->nibble1]
            + chip8->registers->V_REGS[instruction->nibble2];
            VF = chip8->registers->EMU_TEMP[0].word > 255; // set VF to 1 if result greater than 255.
            chip8->registers->V_REGS[instruction->nibble1] // setting Vx to the result AND 255,
            = (uint8_t)(chip8->registers->EMU_TEMP[0].word & 255); // to properly discard bits 8 and up
            break;
        case OP_SUB_VX_VY:
            VF = chip8->registers->V_REGS[instruction->nibble1]
            > chip8->registers->V_REGS[instruction->nibble2];
            chip8->registers->V_REGS[instruction->nibble1]
            -= chip8->registers->V_REGS[instruction->nibble2];
            break;
        case OP_SHR_VX_VY:
            VF = instruction->nibble1 & 0x01;
            chip8->registers->V_REGS[instruction->nibble1] /= 2;
            break;
        case OP_SUBN_VX_VY:
            VF = chip8->registers->V_REGS[instruction->nibble2]
            > chip8->registers->V_REGS[instruction->nibble1];
            chip8->registers->V_REGS[instruction->nibble1]
            = chip8->registers->V_REGS[instruction->nibble2] - chip8->registers->V_REGS[instruction->nibble1];
            break;
        case OP_SHL_VX_VY:
            VF = instruction->nibble1 & 0x01;
            chip8->registers->V_REGS[instruction->nibble1] *= 2;
            break;
        case OP_SNE_VX_VY:
            if (chip8->registers->V_REGS[instruction->nibble1]
            != chip8->registers->V_REGS[instruction->nibble2]) chip8->registers->PC += 2;
            break;
        case OP_LD_I_ADDR:
            chip8->registers->I_REG = PARSE_NNN_TO_U16
            (instruction->nibble1,
            instruction->nibble2,
            instruction->nibble3);
            break;
        case OP_JP_V0_ADDR:
            chip8->registers->PC = PARSE_NNN_TO_U16
            (instruction->nibble1,
             instruction->nibble2,
             instruction->nibble3);
            chip8->registers->PC += chip8->registers->V_REGS[0];
            chip8->registers->PC -= 2;
            break;
        case OP_RND_VX_BYTE:
            TEMP0_BYTE0 = (uint8_t)random_range(0, 255);
            chip8->registers->V_REGS[instruction->nibble1]
            = instruction->bytes[1] & TEMP0_BYTE0;
            break;
        case OP_DRW_VX_VY_NIBBLE:
            // extensive macros to make this more sane to read
#define COL_ORIG chip8->registers->V_REGS[instruction->nibble1]
#define ROW_ORIG chip8->registers->V_REGS[instruction->nibble2]
#define READ_START chip8->registers->I_REG
#define READ_LENGTH instruction->nibble3

#define COL_POS TEMP0_BYTE0 // destination row position
#define ROW_POS TEMP0_BYTE1 // destination col position
#define INDEX TEMP1_BYTE0 // running counter of the write operation
#define BIT_OFFSET TEMP1_BYTE1 // how many bits to the right are we shiting
#define WRITE_POS_LEFT TEMP2_BYTE0 // RAM index to receive the leftmost bits
#define WRITE_POS_RIGHT TEMP2_BYTE1 // RAM index to receive the rightmost bits

            // VF = collision, init to zero, actual check later
            VF = 0;

            // init row coord (AND 31 for wrapping)
            ROW_POS = ROW_ORIG & CHIP8_DISPLAY_Y_MAX;
            // init col coord (AND 63 for wrapping)
            COL_POS = COL_ORIG & CHIP8_DISPLAY_X_MAX;

            // iterating <read length> rows, reading from <read start> RAM location,
            // writing to display starting from <x origin, y origin>
            for (INDEX = 0; INDEX < READ_LENGTH; INDEX++)
            {
                // check offset from 8-bit alignment
                BIT_OFFSET = COL_POS % 8;

                // translating current coords to write position
                WRITE_POS_LEFT = CHIP8_DISPLAY_INDEX(COL_POS, ROW_POS);
                WRITE_POS_RIGHT = (WRITE_POS_LEFT + CHIP8_DISPLAY_HEIGHT);

                // check for collision (if not collided yet)
                VF |= 0 < (chip8->display_memory[WRITE_POS_LEFT]
                                   & (chip8->RAM[READ_START + INDEX] >> BIT_OFFSET));
                VF |= 0 < (chip8->display_memory[WRITE_POS_RIGHT]
                                   & (chip8->RAM[READ_START + INDEX] << (uint8_t)(8 - BIT_OFFSET)));
                // TODO: figure out if this works as well as or better than the following:
                // if (VF == 0) VF
                // = chip8->DISPLAY[WRITE_POS] & chip8->RAM[READ_POS];

                // actually XORing the pixels together
                chip8->display_memory[WRITE_POS_LEFT]
                ^= (chip8->RAM[READ_START + INDEX] >> BIT_OFFSET);
                chip8->display_memory[WRITE_POS_RIGHT]
                ^= (chip8->RAM[READ_START + INDEX] << (uint8_t)(8 - BIT_OFFSET));

                // incrementing y coord for next iteration
                ROW_POS++;

                // if y goes out the bottom it should wrap back from the top,
                // which importantly does NOT affect x
                if (ROW_POS > CHIP8_DISPLAY_Y_MAX)
                {
                    ROW_POS &= CHIP8_DISPLAY_Y_MAX;
                }
            }

            render_display(chip8, window_chip8);
            break;
            // undefining the DRW instruction aliases
#undef COL_ORIG
#undef ROW_ORIG
#undef READ_START
#undef READ_LENGTH
#undef COL_POS
#undef ROW_POS
#undef INDEX
#undef BIT_OFFSET
#undef WRITE_POS_LEFT
#undef WRITE_POS_RIGHT
        case OP_SKP_VX:
            // if key pressed equals value of Vx, skip next instruction
            // TODO: consider combining this instruction and the next with fallthrough
            if (check_input(chip8->emu_state->emu_key_states, instruction->nibble1))
            {
                chip8->registers->PC += 2;
            }
            break;
        case OP_SKNP_VX:
            // if key pressed does not equal value of Vx, skip next instruction
            if (!check_input(chip8->emu_state->emu_key_states, instruction->nibble1))
            {
                chip8->registers->PC += 2;
            }
            break;
        case OP_LD_VX_DT:
            chip8->registers->V_REGS[instruction->nibble1]
            = chip8->registers->DT;
            break;
        case OP_LD_VX_K:
            // LD Vx K - halt execution until any (Chip-8) key is pressed,
            // then store the value of that key in register Vx.

            // some temporary defines for this instruction:
#define INDEX chip8->registers->EMU_TEMP[0].bytes[0]
#define HIGHEST_BUFFER chip8->registers->EMU_TEMP[0].bytes[1]

            // "halt execution" by setting a "loop" flag,
            // which the emulator will enforce by not incrementing any counters,
            // thereby freezing the state and repeating this check until it is satisfied
            chip8->emu_state->loop = true;

            // due to input buffering, it is not enough to check for ANY input,
            // but much preferable to select the OLDEST input (LOWEST buffer frames left),
            // to allow rapid key switching or "typing" if necessary

            HIGHEST_BUFFER = check_input_buffer(chip8->emu_state->chip8_key_states, CHIP8_KEY_ANY_IDX);

            if (HIGHEST_BUFFER > 0)
            {
                for (INDEX = 0; INDEX < CHIP8_KEY_COUNT; INDEX++)
                {
                    if (INDEX == CHIP8_KEY_ANY_IDX) continue;

                    if(check_input_buffer(chip8->emu_state->chip8_key_states, INDEX) == HIGHEST_BUFFER)
                    {
                        Vx = INDEX;
                        chip8->emu_state->loop = false;
                        break;
                    }
                }
            }

            break;
#undef INDEX
#undef HIGHEST_BUFFER
        case OP_LD_DT_VX:
            chip8->registers->DT
            = chip8->registers->V_REGS[instruction->nibble1];
            break;
        case OP_LD_ST_VX:
            chip8->registers->ST
            = chip8->registers->V_REGS[instruction->nibble1];
            break;
        case OP_ADD_I_VX:
            chip8->registers->I_REG
            += chip8->registers->V_REGS[instruction->nibble1];
            break;
        case OP_LD_F_VX:
            // Setting I register to RAM address of <Vx> default sprite
            chip8->registers->I_REG
            = CHIP8_RAM_SPRITES_START
            + (CHIP8_DEFAULT_SPRITE_HEIGHT
            * chip8->registers->V_REGS[instruction->nibble1]);
            break;
        case OP_LD_B_VX:
            // Setting the three bytes starting at RAM <I register> address
            // to the three digits of the Vx integer.
            // TODO: figure out if this is the best way to do this.
            // It's not consequential for CHIP-8, but good to know.
            TEMP0_BYTE0 = chip8->registers->V_REGS[instruction->nibble1];
            TEMP0_BYTE1 = chip8->registers->V_REGS[instruction->nibble1] /= 10;
            TEMP1_BYTE0 = chip8->registers->V_REGS[instruction->nibble1] /= 100;
            chip8->RAM[chip8->registers->I_REG + 2] = TEMP0_BYTE0 % 10;
            chip8->RAM[chip8->registers->I_REG + 1] = TEMP0_BYTE1 % 10;
            chip8->RAM[chip8->registers->I_REG] = TEMP1_BYTE0 % 10;
            break;
        case OP_LD_I_VX:
            for (int i = 0; i <= instruction->nibble1; i++)
            {
                chip8->RAM[chip8->registers->I_REG + i]
                = chip8->registers->V_REGS[i];
            }
            break;
        case OP_LD_VX_I:
            for (int i = 0; i <= instruction->nibble1; i++)
            {
                chip8->registers->V_REGS[i]
                = chip8->RAM[chip8->registers->I_REG + i];
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
#undef Vx
#undef Vy
#undef VF
#undef TEMP0_BYTE0
#undef TEMP0_BYTE1
#undef TEMP1_BYTE0
#undef TEMP1_BYTE1
#undef TEMP0_WORD 
#undef TEMP1_WORD 
}
