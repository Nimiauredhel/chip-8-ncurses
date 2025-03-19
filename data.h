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
    uint16_t PC; // program counter
    uint16_t I_REG; // generally used for RAM addresses
    uint8_t V_REGS[16]; // general purpose registers (except the last one)
    ComboRegister_t EMU_TEMP[3]; // three 16 bit (or six 8 bit) registers I added for the instructions to use
    uint8_t SP; // "stack pointer", holds current index for the return addresses array
    uint16_t STACK_RET[16]; // "stack" - subroutine return addresses
    uint8_t ST; // sound timer
    uint8_t DT; // delay timer
} Chip8Registers_t;

typedef struct EmulatorState
{
    // TODO: compress the separate state flags into a single state enum
    bool should_reset;
    bool step_mode;
    bool step_pressed;

    uint16_t step_delay_us;
    uint32_t step_counter;
    float speed_modifier;
    float seconds_counter;
    float avg_fps;
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
    int key;
    DisplayLayout_t layout;
    uint8_t RAM[CHIP8_RAM_BYTES];
} Chip8_t;

extern const int chip8_key_table[16];
extern const uint8_t chip8_default_sprites[CHIP8_DEFAULT_SPRITES_SIZE];

Chip8_t *create_instance(char *rom_path);
void load_default_sprites(Chip8_t *chip8);
size_t load_rom(char *rom_path, uint8_t *dest_ptr);

#endif
