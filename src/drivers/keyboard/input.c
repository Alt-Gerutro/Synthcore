#include <drivers/keyboard/input.h>
#include <drivers/video/screen.h>
#include <arch/x86/interrupts/interrupts.h>
#include <arch/x86/interrupts/isr.h>

char* find_key_by_scancode(uint8_t sc) {
    uint32_t k_size = sizeof(keys) / sizeof(keys[0]);
    for (uint32_t i = 0; i < k_size; i++) {
        if (keys[i].scancode == sc) {
            return keys[i].string;
        }
    }
    return "\0";
}

void keyboard_init() {
    idt_set_gate(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);
}