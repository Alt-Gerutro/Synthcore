#include "drivers/int/idt_d.h"
#include "drivers/int/pic_d.h"
#include <drivers/io/input/input_d.h>
#include <drivers/io/screen/screen_d.h>
#include <drivers/int/interrupts_h.h>

char* find_key_by_scancode(uint8_t sc) {
    uint32_t k_size = sizeof(keys) / sizeof(keys[0]);
    for (uint32_t i = 0; i < k_size; i++) {
        if (keys[i].scancode == sc) {
            return keys[i].string;
        }
    }
    return "\0";
}
void keyboard_interrupt_body() {
    uint8_t sc = inB(REG_INPUT_DATA);
    uint8_t attr = make_color(VGA_BYTE_BLACK, VGA_BYTE_LIGHT_GREY);

    if (sc < 0x80) {
        char* key = find_key_by_scancode(sc);
        switch (sc) {
        case 0x0E:
            backspace();
            break;
        case 0x1C:
            put_char('\n', 0x0F);
            break;
        default:
            print_str(key, attr);
            break;
        }
    }
    sendEOI(1);
}

__attribute__((naked))
void isr_keyboard() {
    __asm__ volatile(
        "pusha\n"
        "call keyboard_interrupt_body\n"
        "popa\n"
        "iret"
    );
}

void keyboard_init() {
    idt_set_gate(0x21, (uint32_t)isr_keyboard, 0x08, 0x8E);
}