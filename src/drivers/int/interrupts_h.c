#include <drivers/int/interrupts_h.h>

void int_init() {
    gdt_init();
    idt_init();
    pic_remap(0x20, 0x28);
}