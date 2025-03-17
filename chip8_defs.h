#ifndef CHIP8_DEFS_H
#define CHIP8_DEFS_H

#define CHIP8_INSTRUCTION_COUNT 46

#define CHIP8_DEFAULT_PROGRAM_START (0x200)
#define CHIP8_DEFAULT_SPRITES_START (0x050)
#define CHIP8_DEFAULT_SPRITES_COUNT (16)
#define CHIP8_DEFAULT_SPRITE_HEIGHT (5)
#define CHIP8_DEFAULT_SPRITES_SIZE (CHIP8_DEFAULT_SPRITES_COUNT * CHIP8_DEFAULT_SPRITE_HEIGHT)
#define CHIP8_DEFAULT_SPRITES_END (CHIP8_DEFAULT_SPRITES_START + CHIP8_DEFAULT_SPRITES_SIZE)

#define CHIP8_DISPLAY_WIDTH (64)
#define CHIP8_DISPLAY_HEIGHT (32)
#define CHIP8_DISPLAY_ROW_BYTES (CHIP8_DISPLAY_WIDTH / 8)
#define CHIP8_DISPLAY_X_MAX (CHIP8_DISPLAY_ROW_BYTES - 1)
#define CHIP8_DISPLAY_Y_MAX (CHIP8_DISPLAY_HEIGHT - 1)
#define CHIP8_DISPLAY_SIZE (CHIP8_DISPLAY_ROW_BYTES * CHIP8_DISPLAY_HEIGHT)
#define CHIP8_DISPLAY_INDEX(x, y) (y + (x * CHIP8_DISPLAY_HEIGHT))

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

#define EMU_DEFAULT_DELAY_US (16666) // 60 hz
#define EMU_SPEED_INCREMENT (0.1f)
#define EMU_MIN_SPEED_MOD (0.0f) // 0 hz
#define EMU_MAX_SPEED_MOD (100.0f) // 6000 hz

#define EMU_KEY_RESET ('r')
#define EMU_KEY_STEP_MODE ('s')
#define EMU_KEY_STEP_ONE (' ')

#endif
