#include <drivers/io/base_io.h>
#include <drivers/interrupts/idt.h>

typedef struct {
    u16 scancode;
    char *string;
} keyPair;

keyPair keys[] = {
    {0x1, "esc"}, {0x3B, "F1"}, {0x3C, "F2"}, {0x3D, "F3"}, {0x3E, "F4"}, {0x3F, "F5"}, {0x40, "F6"}, {0x41, "F7"}, {0x42, "F8"}, {0x43, "F9"}, {0x44, "F10"}, {0x57, "F11"}, {0x58, "F12"},

    {0x29, "`"}, {0x2, "1"}, {0x3, "2"}, {0x4, "3"}, {0x5, "4"}, {0x6, "5"}, {0x7, "6"}, {0x8, "7"}, {0x9, "8"}, {0xA, "9"}, {0xB, "0"}, {0x0C, "-"}, {0x0D, "="},
    {0x10, "q"}, {0x11, "w"}, {0x12, "e"}, {0x13, "r"}, {0x14, "t"}, {0x15, "y"}, {0x16, "u"}, {0x17, "i"}, {0x18, "o"}, {0x19, "p"}, {0x1A, "["}, {0x1B, "]"},
    {0x1E, "a"}, {0x1F, "s"}, {0x20, "d"}, {0x21, "f"}, {0x22, "g"}, {0x23, "h"}, {0x24, "j"}, {0x25, "k"}, {0x26, "l"}, {0x27, ";"}, {0x28, "\'"},
    {0x2C, "z"}, {0x2D, "x"}, {0x2E, "c"}, {0x2F, "v"}, {0x30, "b"}, {0x31, "n"}, {0x32, "m"}, {0x33, ","}, {0x34, "."}, {0x35, "/"},

    {0x0E, "backspace"}, {0x0F, "tab"}, {0x1C, "enter"}, {0x3A, "caps"},
    {0x2A, "L shift"}, {0x36, "R shift"},
    {0x1D, "ctrl"}, {0x5B, "meta"}, {0x38, "alt"},
    {0x39, " "}
};

char *find_key_by_scancode(u8 sc) {
    int k_size = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < k_size; i++) {
        if (keys[i].scancode == sc) {
            return keys[i].string;
        }
    }
    return "\0";
}

char get_char(u16 pos) {
    char c;
    pos *= 2;
    u8 *vga = (u8 *)VIDEO_ADDRESS;
    c = vga[pos];

    return c;
}

u8 get_attr(u16 pos) {
    u8 c;
    u16 cursor = pos;
    cursor *=2;
    pos *= 2;

    u8 *vga = (u8 *)VIDEO_ADDRESS;
    c = vga[cursor+1];

    return c;
}

void backspace() {
    u16 cursor = get_cursor();
    if (cursor == 0) return;

    u8 at_start_line = (cursor % MAX_COLS == 0) ? 1: 0;

    if (!at_start_line) {
        write('\0', 0x0F, (cursor-2));
        set_cursor(cursor-2);
    } else {
        u16 offset = cursor;
        while(get_char(offset-1) == '\0') {
            offset--;
        }
        set_cursor(offset);
    }
}

__attribute__((naked)) static void keyboard_isr() {
    asm volatile (
        "pusha\n\t"
        "call keyboard_handler\n\t"
        "popa\n\t"
        "iret"
    );
}

void keyboard_init() {
    idt_init();
    idt_set_gate(0x21, keyboard_isr, 0x08, 0x8E);

    lidt(idt, sizeof(idt) - 1);
    sti();
}

void keyboard_handler() {
    u8 sc = inB(REG_INPUT_DATA);
    u8 style = 0x0F;
    if (sc < 0x80) {
        char *key = find_key_by_scancode(sc);
        switch (sc) {
        case 0x0E:
            backspace();
            break;
        case 0x1C:
            put_char('\n', 0x0F);
            break;
        default:
            print_str(key, style);
            break;
        }
    }
    outB(0x20, 0x20);
}
