#include <drivers/io/base_io.h>
#include <drivers/interrupts/idt.h>

typedef struct {
    u16 hex;
    char character;
} scodes;

scodes keys[] = {
    {0x00, 0}, {0x10, 'q'}, {0x20, '1'}, {0x30, '2'}, {0x01, '3'}, {0x11, '4'}, {0x21, '5'}, {0x31, '6'}, {0x02, '7'}, {0x12, '8'}, {0x22, '9'}, {0x32, '0'}, {0x03, '-'}, {0x13, '='},
    {0x23, 0}, {0x33, 0},
    {0x04, 'q'}, {0x14, 'w'}, {0x24, 'e'}, {0x34, 'r'}, {0x05, 't'}, {0x15, 'y'}, {0x25, 'u'}, {0x35, 'i'}, {0x06, 'o'}, {0x16, 'p'}, {0x26, '['}, {0x36, ']'},
    {0x07, 0}, {0x17, 'i'}, {0x27, 'a'}, {0x37, 's'},
    {0x08, 'd'}, {0x18, 'f'}, {0x28, 'g'}, {0x38, 'h'},
    {0x09, 'j'}, {0x19, 'k'}, {0x29, 'l'}, {0x39, ';'},
    {0x0A, '\''}, {0x1A, 0}, {0x2A, 'v'}, {0x3A, 0},
    {0x0B, 'z'}, {0x1B, 'x'}, {0x2B, 'c'}, {0x3B, 'v'},
    {0x0C, 'b'}, {0x1C, 'n'}, {0x2C, 'm'}, {0x3C, ','},
    {0x0D, '.'}, {0x1D, '/'}, {0x2D, 0}, {0x3D, 0},
    {0x0E, 0}, {0x1E, 0}, {0x2E, 0}, {0x3E, 0},
    {0x0F, 0}, {0x1F, 0}, {0x2F, 0}, {0x3F, 0},
};

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
    if (sc < 0x80) {
        char key = "??1234567890-=??qwertyuiop[]\n?asdfghjkl;'`?\\zxcvbnm,./???"[sc];
        //char key = keys[sc].character;
        if (sc == 0x0E) {
            u16 cursor = get_cursor();
            write('\0', 0x0F, (cursor-2));
            set_cursor(cursor-2);
        } else if (sc == 0x07) {
            outB(0x20, 0x20);
        } else {
            put_char(key, 0x0F);
        }
    }
    outB(0x20, 0x20);
}
