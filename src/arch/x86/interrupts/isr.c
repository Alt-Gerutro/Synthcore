#include <arch/x86/interrupts/isr.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <utils/byte_io.h>
#include <drivers/video/screen.h>
#include <drivers/keyboard/input.h>
#include <arch/x86/interrupts/pic.h>

void default_handler_body() {}
__attribute__((naked))
void default_handler() {
    __asm__ volatile(
        "pusha\n"
        "call default_handler_body\n"
        "popa\n"
        "iret"
    );
}

void keyboard_handler_body() {
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
void keyboard_handler() {
    __asm__ volatile(
        "pusha\n"
        "call keyboard_handler_body\n"
        "popa\n"
        "iret"
    );
}
