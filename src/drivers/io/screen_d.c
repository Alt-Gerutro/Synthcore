#include <stdint.h>
#include <drivers/io/screen/screen_d.h>
#include <utils/byte_io.h>

void int_to_str(int32_t num, char* str) {
    uint32_t i = 0;
    uint8_t is_neg = 0;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (num < 0) {
        is_neg = 1;
    } else {
        num = -num;
    }

    while (num < 0) {
        int32_t digit = -(num%10);
        str[i++] = digit + '0';
        num /= 10;
    }

    if (is_neg) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int32_t start = 0;
    int end = i - 1;
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
    position /= 2;  // <EN: Transform position to byte shift>
                    // <RU: Преобразование позиции в байтовый сдвиг>

    outB(REG_VGA_CTRL, 14);                         // <EN: Send msg 0b1110 to get higher byte of cursor>
    outB(REG_VGA_DATA, (uint8_t)(position >> 8));
    outB(REG_VGA_CTRL, 15);                         // Send msg 0b1111 to get lower byte of cursor
    outB(REG_VGA_DATA, (uint8_t)(position & 0xFF));
}

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
        write('\0', VGA_BYTE_BLACK | VGA_BYTE_WHITE, (last_line + i * 2));
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

    // uint8_t at_start_line = (cursor % MAX_COLS == 0) ? 1 : 0;

    // if (!at_start_line) {
        write('\0', make_color(VGA_BYTE_BLACK, VGA_BYTE_WHITE), (cursor - 2));
        set_cursor(cursor - 2);
    // } else {
    //     uint16_t offset = cursor;
    //     while(get_char(offset - 1) == '\0') {
    //         offset--;
    //     }
    //     set_cursor(offset);
    // }
}