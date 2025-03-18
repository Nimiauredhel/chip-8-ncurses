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
    bool should_reset;
    bool step_mode;
    bool step_pressed;

    float speed_modifier;
    uint32_t tick_increment;
    uint32_t tick_counter;
    uint32_t frame_counter;
    float seconds_counter;
    float avg_fps;

    int key;
} EmulatorState_t;

typedef struct Chip8
{
    EmulatorState_t *emu_state;
    Chip8Registers_t *registers;
    uint8_t *display_memory;
    Chip8Instruction_t *instruction;
    uint8_t RAM[CHIP8_RAM_BYTES];
} Chip8_t;

extern const uint8_t default_sprites[CHIP8_DEFAULT_SPRITES_SIZE];

Chip8_t *create_instance(char *rom_path);
void load_default_sprites(Chip8_t *chip8);
size_t load_rom(char *rom_path, uint8_t *dest_ptr);

#endif
