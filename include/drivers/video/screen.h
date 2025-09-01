#ifndef _DRIVERS_SCREEN_SYNTHCORE_H
#define _DRIVERS_SCREEN_SYNTHCORE_H

#include <stdarg.h>
#include <stdint.h>

#define NULL    ((void*)0)

#define VGA_ADDRESS     0xB8000
#define MAX_ROWS        25
#define MAX_COLS        80
#define MAX_CHARS       MAX_ROWS * MAX_COLS
#define MAX_BYTE_OFFSET MAX_CHARS * 2

#define REG_VGA_CTRL    0x3D4
#define REG_VGA_DATA    0x3D5

/*
 * <EN:
 * Colors in bytes.>
 * <RU:
 * Цвета в байтах.>
 * <EXAMPLES:
 * 0x0F - black background, white symbol;
 * 0xF0 - white background, black symbol.>
 */
enum color_byte {
    VGA_BYTE_BLACK = 0x0,
    VGA_BYTE_DARK_BLUE = 0x1,
    VGA_BYTE_GREEN = 0x2,
    VGA_BYTE_DARK_CYAN = 0x3,
    VGA_BYTE_DARK_RED = 0x4,
    VGA_BYTE_PURPLE = 0x5,
    VGA_BYTE_BROWN = 0x6,
    VGA_BYTE_LIGHT_GREY = 0x7,
    VGA_BYTE_GREY = 0x8,
    VGA_BYTE_BLUE = 0x9,
    VGA_BYTE_LIME = 0xA,
    VGA_BYTE_CYAN = 0xB,
    VGA_BYTE_LIGHT_RED = 0xC,
    VGA_BYTE_MAGENTA = 0xD,
    VGA_BYTE_YELLOW = 0xE,
    VGA_BYTE_WHITE = 0xF
};

void int_to_str(int32_t num, char* str);

uint8_t make_color(uint8_t background, uint8_t foreground);

/*
 * Set cursor on position.
 * Position - index on screen in range from 0 to 80*25(not byte shift)
 */
void set_cursor(uint16_t position);

/*
 * Get cursor position.
 * Position - index on screen in range from 0 to 80*25(not byte shift)
 */
uint16_t get_cursor();


void write(char character, uint8_t attribute_byte, uint16_t offset);
void scroll_line();

void put_char(char character, uint8_t attribute_byte);
void print_str(char* str, uint8_t attribute_byte);

void clear_screen();

char get_char(uint16_t pos);
uint8_t get_attr(uint16_t pos);

void backspace();

void vformat(char buffer[], uint32_t buffer_size, char* fmt, va_list args);
void print_fmt(char* fmt, ...);

#endif // _DRIVERS_INPUT_SYNTHCORE_H