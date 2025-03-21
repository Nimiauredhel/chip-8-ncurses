#ifndef DATA_H
#define DATA_H

#include "common.h"
#include "parser.h"
#include "chip8_defs.h"

typedef union ComboRegister
{
    uint8_t bytes[2];
    uint16_t word;
} ComboRegister_t;

typedef struct Chip8Registers
{
    uint8_t ST; // sound timer
    uint8_t DT; // delay timer
    uint8_t SP; // "stack pointer", holds current index for the return addresses array
    uint16_t PC; // program counter
    uint16_t I_REG : 12; // generally used for RAM addresses
    uint8_t V_REGS[16]; // general purpose registers (except the last one)
    uint16_t STACK_RET[16]; // "stack" - subroutine return addresses
    ComboRegister_t EMU_TEMP[3]; // three 16 bit (or six 8 bit) registers I added for the instructions to use
} Chip8Registers_t;

typedef struct EmulatorState
{
    // TODO: compress the separate state flags into a single state enum
    bool should_reset;
    bool loop;
    bool step_mode;
    bool step_pressed;

    struct timespec start_clock;
    uint16_t emu_key_states[EMU_KEY_COUNT];
    uint16_t chip8_key_states[CHIP8_KEY_COUNT];
    uint16_t ideal_step_delay_us;
    uint16_t difference_step_delay_us;
    uint32_t step_counter;
    float speed_modifier;
    float runtime_seconds_counter;
    float cycle_seconds_counter;
    float avg_cps;
} EmulatorState_t;

typedef struct DisplayLayout
{
    WINDOW *window_chip8;
    WINDOW *window_emu;
    WINDOW *window_disassembly;
    WINDOW *window_registers;
} DisplayLayout_t;

typedef struct Chip8
{
    EmulatorState_t *emu_state;
    Chip8Registers_t *registers;
    uint8_t *display_memory;
    Chip8Instruction_t *instruction;
    DisplayLayout_t layout;
    uint8_t RAM[CHIP8_RAM_BYTES];
} Chip8_t;

extern const uint8_t chip8_default_sprites[CHIP8_DEFAULT_SPRITES_SIZE];

Chip8_t *create_instance(char *rom_path);
void load_default_sprites(Chip8_t *chip8);
size_t load_rom(char *rom_path, uint8_t *dest_ptr);

#endif
