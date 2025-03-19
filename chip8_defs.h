#ifndef CHIP8_DEFS_H
#define CHIP8_DEFS_H

// *** VM definitions ***

// emulator constants
#define EMU_DEFAULT_STEP_DELAY_US (16666) // 60hz execution step frequency
#define EMU_SPEED_INCREMENT (0.1f)
#define EMU_MIN_SPEED_MOD (0.0f) // 0 hz
#define EMU_MAX_SPEED_MOD (4.0f) // "240hz"

// emulator controls
#define EMU_KEY_NEUTRAL ('~') // tilde - unused, only signifies "no key pressed"
#define EMU_KEY_SPEED_UP ('+') // Shift-Plus
#define EMU_KEY_SPEED_DOWN ('_') // Shift-Minus 
#define EMU_KEY_RESET ('|') // Shift-\ -terminate the current instance and immediately launch another
#define EMU_KEY_STEP_MODE ('P') // Shift-P - enter/exit step mode
#define EMU_KEY_STEP_ONE ('p') // P - execute a single step in step mode

// *** CHIP-8 definitions ***

#define CHIP8_INSTRUCTION_COUNT (46)

// chip-8 controls
#define CHIP8_KEY_1 ('1')
#define CHIP8_KEY_2 ('2')
#define CHIP8_KEY_3 ('3')
#define CHIP8_KEY_C ('4')
#define CHIP8_KEY_4 ('q')
#define CHIP8_KEY_5 ('w')
#define CHIP8_KEY_6 ('e')
#define CHIP8_KEY_D ('r')
#define CHIP8_KEY_7 ('a')
#define CHIP8_KEY_8 ('s')
#define CHIP8_KEY_9 ('d')
#define CHIP8_KEY_E ('f')
#define CHIP8_KEY_A ('z')
#define CHIP8_KEY_0 ('x')
#define CHIP8_KEY_B ('c')
#define CHIP8_KEY_F ('v')

// map of VM memory utilization up to the 0x200 program start point
// TODO: maybe tighten this up later when the sizes are stable
#define CHIP8_RAM_BYTES (4096)
#define CHIP8_RAM_EMU_STATE_START (0x000) // 0 - ??<80, ??<80B
#define CHIP8_RAM_SPRITES_START   (0x050) // 80 - 159, 80B
#define CHIP8_RAM_DISPLAY_START   (0x0A0) // 160 - 415, 256B
#define CHIP8_RAM_REGISTERS_START   (0x1A4) // 420 - ~481, ~62B
#define CHIP8_RAM_INSTRUCTION_START   (0x1F4) // 500 - 511, 12B parsed instruction storage
#define CHIP8_RAM_PROGRAM_START   (0x200) // 512 - 4095

// sprites
#define CHIP8_DEFAULT_SPRITES_COUNT (16)
#define CHIP8_DEFAULT_SPRITE_HEIGHT (5)
#define CHIP8_DEFAULT_SPRITES_SIZE (CHIP8_DEFAULT_SPRITES_COUNT * CHIP8_DEFAULT_SPRITE_HEIGHT)
#define CHIP8_DEFAULT_SPRITES_END (CHIP8_DEFAULT_SPRITES_START + CHIP8_DEFAULT_SPRITES_SIZE)

// display
#define CHIP8_DISPLAY_WIDTH (64)
#define CHIP8_DISPLAY_HEIGHT (32)
#define CHIP8_DISPLAY_ROW_BYTES (CHIP8_DISPLAY_WIDTH / 8)
#define CHIP8_DISPLAY_X_MAX (CHIP8_DISPLAY_WIDTH - 1)
#define CHIP8_DISPLAY_Y_MAX (CHIP8_DISPLAY_HEIGHT - 1)
#define CHIP8_DISPLAY_ROW_BYTE_MAX (CHIP8_DISPLAY_ROW_BYTES - 1)
#define CHIP8_DISPLAY_INDEX(x, y) (y + ((x / 8) * CHIP8_DISPLAY_HEIGHT))
#define CHIP8_DISPLAY_BYTES (CHIP8_DISPLAY_ROW_BYTES * CHIP8_DISPLAY_HEIGHT)

       // col = idx / CHIP8_DISPLAY_HEIGHT;
       // row = idx % CHIP8_DISPLAY_HEIGHT;
// super chip 48 defines are commented out for convenience
// until I actually implement super chip 48 support
/*
#define CHIP8_DISPLAY_SUPER_WIDTH 128
#define CHIP8_DISPLAY_SUPER_HEIGHT 64
#define CHIP8_DISPLAY_SUPER_X_MAX (CHIP8_DISPLAY_SUPER_WIDTH - 1)
#define CHIP8_DISPLAY_SUPER_Y_MAX (CHIP8_DISPLAY_SUPER_HEIGHT - 1)
#define CHIP8_DISPLAY_SUPER_SIZE CHIP8_DISPLAY_SUPER_WIDTH * CHIP8_DISPLAY_SUPER_HEIGHT
#define CHIP8_DISPLAY_SUPER_INDEX(x, y) (y + (x * CHIP8_DISPLAY_SUPER_HEIGHT))
*/

#endif
