#include <__stdarg_va_list.h>
#include <drivers/video/screen.h>
#include <stdarg.h>
#include <utils/byte_io.h>
#include <stdint.h>

void int_to_str(int32_t num, char* str) {
    uint32_t i = 0;
    uint8_t is_negative = 0;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (num < 0) {
        is_negative = 1;
    } else {
        num = -num;
    }

    while (num < 0) {
        int32_t digit = -(num%10);
        str[i++] = digit + '0';
        num /= 10;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int32_t start = 0;
    int32_t end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

uint8_t make_color(uint8_t background, uint8_t foreground) {
    return ((background << 4) | foreground);
}

void set_cursor(uint16_t position) {
    position /= 2;  /* Transform position to byte shift */

    outB(REG_VGA_CTRL, 14);                         /* Send msg 0b1110 to get higher byte of cursor */
    outB(REG_VGA_DATA, (uint8_t)(position >> 8));
    outB(REG_VGA_CTRL, 15);                         /* Send msg 0b1111 to get lower byte of cursor */
    outB(REG_VGA_DATA, (uint8_t)(position & 0xFF));
}

/* 
 * Function to get cursor.
 * Return byte offset.
 */
uint16_t get_cursor() {
    uint16_t hb, lb;

    outB(REG_VGA_CTRL, 14);
    hb = inB(REG_VGA_DATA);
    outB(REG_VGA_CTRL, 15);
    lb = inB(REG_VGA_DATA);

    return (((hb << 8) + lb) * 2);
}

void write(char character, uint8_t attribute_byte, uint16_t offset) {
    uint8_t* vga = (uint8_t*)VGA_ADDRESS;
    vga[offset] = character;
    vga[offset+1] = attribute_byte;
}

void scroll_line() {
    uint8_t i = 1;
    uint16_t last_line;

    while (i < MAX_ROWS) {
        mem_copy(
            (uint8_t*)(VGA_ADDRESS + (MAX_COLS * i * 2)),
            (uint8_t*)(VGA_ADDRESS + (MAX_COLS * (i-1) * 2)),
            (MAX_COLS * 2)
        );
        i++;
    }

    last_line = (MAX_BYTE_OFFSET - MAX_COLS * 2);
    i = 0;

    while (i < MAX_COLS) {
        write('\0', make_color(VGA_BYTE_BLACK, VGA_BYTE_WHITE), (last_line + i * 2));
        i++;
    }

    set_cursor(last_line);
}

void put_char(char character, uint8_t attribute_byte) {
    uint16_t offset = get_cursor();

    if (character == '\n') {
        if ((offset / 2 / MAX_COLS) == (MAX_ROWS - 1)) scroll_line();
        else set_cursor((offset - offset % (MAX_COLS * 2)) + MAX_COLS * 2);
    } else {
        if (offset == MAX_BYTE_OFFSET) scroll_line();
        write(character, attribute_byte, offset);
        set_cursor(offset + 2);
    }
}

void print_str(char *str, uint8_t attribute_byte) {
    while (*str) {
        put_char(*str, attribute_byte);
        str++;
    }
}

void clear_screen() {
    for (uint16_t i = 0; i < MAX_BYTE_OFFSET; i++) {
        write('\0', VGA_BYTE_BLACK | VGA_BYTE_WHITE, i);
    }
}

char get_char(uint16_t pos) {
    char c;
    pos *= 2;

    uint8_t* vga = (uint8_t*)VGA_ADDRESS;
    c = vga[pos];

    return c;
}

uint8_t get_attr(uint16_t pos) {
    uint8_t attr;
    pos *= 2;
    uint8_t* vga = (uint8_t*)VGA_ADDRESS;
    attr = vga[pos+1];

    return attr;
}

void backspace() {
    uint16_t cursor = get_cursor();
    if (cursor == 0) return;

    write('\0', make_color(VGA_BYTE_BLACK, VGA_BYTE_WHITE), (cursor - 2));
    set_cursor(cursor - 2);
}

void vformat(char buffer[], uint32_t buffer_size, char* fmt, va_list args) {
    uint32_t i = 0; /* For index in fmt */
    uint32_t j = 0; /* For index in out */
    
    while (fmt[i] && j < buffer_size - 1) {
        if(fmt[i] != '%') {
            buffer[j++] = fmt[i++];
        } else {
            i++;
            switch (fmt[i]) {
                case 's': {
                    char* str = va_arg(args, char*);
                    while (*str && j < buffer_size - 1) {
                        buffer[j++] = *str++;
                    }
                    break;
                }
                case 'd': {
                    char buf[12];
                    uint32_t num = va_arg(args, int);
                    int_to_str(num, buf);
                    uint32_t k = 0;
                    while (buf[k] && j < buffer_size - 1) {
                        buffer[j++] = buf[k++];
                    }
                    break;
                }
                default: {
                    if(j < buffer_size - 1) {
                        buffer[j++] = fmt[i];
                    }
                    break;
                }
            }
            i++;
        }
    }
    buffer[j++] = '\0';
}

void print_fmt(char* fmt, ...) {
    uint32_t buf_size = 512;
    if (sizeof(fmt) >= buf_size - 1) {
        buf_size += 512;
    }
    char buf[buf_size];
    va_list args;
    va_start(args, fmt);
    vformat(buf, buf_size, fmt, args);
    va_end(args);
    print_str(buf, make_color(VGA_BYTE_BLACK, VGA_BYTE_WHITE));
}