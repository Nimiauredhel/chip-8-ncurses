#include "input.h"

char chip8_key_names[CHIP8_KEY_COUNT][4] =
{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "Any" };

char emu_key_names[EMU_KEY_COUNT][10] =
{ "Quit", "Reset", "Speed +", "Speed -", "Step Mode", "Step One", "Any" };

void read_input(uint16_t *emu_key_states, uint16_t *chip8_key_states)
{
    KeyBind_t key = KEY_NONE;
    uint8_t idx;

    for (idx = 0; idx < CHIP8_KEY_COUNT; idx++)
    {
        if (chip8_key_states[idx] > 0)
        chip8_key_states[idx] -= 1;
    }

    for (idx = 0; idx < EMU_KEY_COUNT; idx++)
    {
        if (emu_key_states[idx] > 0)
        emu_key_states[idx] -= 1;
    }

    // loop over several getch() calls to allow concurrent keypresses
    for (uint8_t i = 0; i < MAX_SIMULTANEOUS_KEYS; i++)
    {
#define REGISTER_EMU_KEY_DOWN(key)\
        emu_key_states[key] = EMU_INPUT_BUFFER_CYCLES;\
        emu_key_states[EMU_KEY_ANY_IDX] = EMU_INPUT_BUFFER_CYCLES
#define REGISTER_CHIP8_KEY_DOWN(key)\
        chip8_key_states[key] = CHIP8_INPUT_BUFFER_CYCLES;\
        chip8_key_states[CHIP8_KEY_ANY_IDX] = CHIP8_INPUT_BUFFER_CYCLES

        key = getch();

        switch(key)
        {
            default:
            case KEY_NONE:
                break;
            case EMU_KEY_QUIT_BIND:
                REGISTER_EMU_KEY_DOWN(EMU_KEY_QUIT_IDX);
                break;
            case EMU_KEY_SPEED_UP_BIND:
                REGISTER_EMU_KEY_DOWN(EMU_KEY_SPEED_UP_IDX);
                break;
            case EMU_KEY_SPEED_DOWN_BIND:
                REGISTER_EMU_KEY_DOWN(EMU_KEY_SPEED_DOWN_IDX);
                break;
            case EMU_KEY_RESET_BIND:
                REGISTER_EMU_KEY_DOWN(EMU_KEY_RESET_IDX);
                break;
            case EMU_KEY_STEP_MODE_BIND:
                REGISTER_EMU_KEY_DOWN(EMU_KEY_STEP_MODE_IDX);
                break;
            case EMU_KEY_STEP_ONE_BIND:
                REGISTER_EMU_KEY_DOWN(EMU_KEY_STEP_ONE_IDX);
                break;
            case CHIP8_KEY_1_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_1_IDX);
                break;
            case CHIP8_KEY_2_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_2_IDX);
                break;
            case CHIP8_KEY_3_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_3_IDX);
                break;
            case CHIP8_KEY_C_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_C_IDX);
                break;
            case CHIP8_KEY_4_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_4_IDX);
                break;
            case CHIP8_KEY_5_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_5_IDX);
                break;
            case CHIP8_KEY_6_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_6_IDX);
                break;
            case CHIP8_KEY_D_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_D_IDX);
                break;
            case CHIP8_KEY_7_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_7_IDX);
                break;
            case CHIP8_KEY_8_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_8_IDX);
                break;
            case CHIP8_KEY_9_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_9_IDX);
                break;
            case CHIP8_KEY_E_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_E_IDX);
                break;
            case CHIP8_KEY_A_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_A_IDX);
                break;
            case CHIP8_KEY_0_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_0_IDX);
                break;
            case CHIP8_KEY_B_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_B_IDX);
                break;
            case CHIP8_KEY_F_BIND:
                REGISTER_CHIP8_KEY_DOWN(CHIP8_KEY_F_IDX);
                break;
            }
#undef REGISTER_KEY_DOWN
    }
}

bool check_input(uint16_t *key_states, uint8_t key_idx)
{
    bool result = key_states[key_idx] > 0;
    return result;
}

uint16_t check_input_buffer(uint16_t *key_states, uint8_t key_idx)
{
    uint8_t result = key_states[key_idx];
    return result;
}
