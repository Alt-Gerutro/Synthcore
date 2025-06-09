#include <drivers/io/base_io.h>

/* Установка курсора на определённую позицию */
void set_cursor(u16 pos) {
    pos /= 2; // Преобразуем pos в char_offset

    outB(REG_SCREEN_CTRL, 14);
    outB(REG_SCREEN_DATA, (u8)(pos >> 8));
    outB(REG_SCREEN_CTRL, 15);
    outB(REG_SCREEN_DATA, (u8)(pos & 0xff));
}

u16 get_cursor() {
    u16 hb, lb;

    outB(REG_SCREEN_CTRL, 14);
    hb = inB(REG_SCREEN_DATA);
    outB(REG_SCREEN_CTRL, 15);
    lb = inB(REG_SCREEN_DATA);

    return (((hb << 8) + lb) * 2);
}

void write(char character, u8 attr, u16 offset) {
    u8 *vga = (u8 *) VIDEO_ADDRESS;
    vga[offset] = character;
    vga[offset+1] = attr;
}

void clear_screen() {
    u32 offset = 0;
    while (offset < MAX_BYTE_CHARS) {
        write('\0', 0x0F, offset);
        offset += 2;
    }
    set_cursor(0);
}

void scroll_line() {
    u8 i = 1;
    u16 last_line;

    while (i < MAX_ROWS) {
        mem_copy(
            (u8 *)(VIDEO_ADDRESS + (MAX_COLS * i * 2)),
            (u8 *)(VIDEO_ADDRESS + (MAX_COLS * (i-1) * 2)),
            (MAX_COLS * 2)
        );
        i++;
    }

    last_line = (MAX_BYTE_CHARS - MAX_COLS * 2);
    i = 0;

    while (i < MAX_COLS) {
        write('\0', 0x0F, (last_line + i * 2));
        i++;
    }

    set_cursor(last_line);
}

void put_char(char character, u8 attr) {
    u16 offset = get_cursor();

    if (character == '\n') {
        if ( (offset / 2 / MAX_COLS) == (MAX_ROWS - 1) ) scroll_line();
        else set_cursor( (offset - offset % (MAX_COLS * 2) ) + MAX_COLS * 2);
    } else {
        if ( offset == MAX_BYTE_CHARS ) scroll_line();
        write(character, attr, offset);
        set_cursor(offset + 2);
    }
}

void print_str(char str[], u8 attr) {
    while (*str) {
        put_char(*str, attr);
        str++;
    }
}
