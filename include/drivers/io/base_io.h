#ifndef _IO_DRIVERS_DISPLAY_SYNTHCORE
#define _IO_DRIVERS_DISPLAY_SYNTHCORE

#include <utils/types.h>
#include <utils/byte_io.h>

#define VIDEO_ADDRESS   0xB8000
#define MAX_ROWS        25
#define MAX_COLS        80
#define MAX_CHARS       MAX_COLS * MAX_ROWS
#define MAX_BYTE_CHARS  MAX_CHARS * 2

/*
*   Colors of bits in ASCII:
*   0 - BLACK;
*   1 - DARK BLUE;
*   2 - GREEN;
*   3 - DARK CYAN;
*   4 - DARK RED;
*   5 - PURPLE;
*   6 - BROWN;
*   7 - LIGHT GRAY;
*   8 - GRAY
*   9 - BLUE;
*   A - LIME;
*   B - CYAN;
*   C - LIGHT RED;
*   D - MAGNETA;
*   E - YELLOW;
*   F - WHITE.
*
*   Examples:
*   0x0F - black background, white symbol.
*   0xE0 - yellow background, black symbol.
*/

#define REG_SCREEN_CTRL     0x3D4
#define REG_SCREEN_DATA     0x3D5

#define REG_INPUT_DATA      0x60

void set_cursor(u16 pos);
u16 get_cursor();

void write(char character, u8 attr, u16 offset);
void clear_screen();
void scroll_line();

void put_char(char character, u8 attr);
void print_str(char str[], u8 attr);

void keyboard_init();
void keyboard_handler();

void backspace();
char get_char(u16 pos);
u8 get_attr(u16 pos);

#endif // _IO_DRIVERS_DISPLAY_SYNTHCORE
