#include <drivers/int/idt_d.h>
#include <stdint.h>
#include <drivers/io/screen/screen_d.h>

struct IDT_entry idt[IDT_SIZE];
struct IDT_ptr idtp;

void default_handler_body() {}

void default_handler() {
    __asm__ volatile(
        "pusha\n"
        "call default_handler_body\n"
        "popa\n"
        "iret"
    );
}

void idt_set_gate(uint8_t vector, uint32_t handler, uint8_t selector, uint8_t flags) {
    idt[vector].offset_low = handler & 0xFFFF;
    idt[vector].offset_high = (handler >> 16) & 0xFFFF;
    idt[vector].selector = selector;
    idt[vector].reserved = 0;
    idt[vector].type_attr = flags;
}

void idt_init() {
    for(uint16_t i = 0; i < 256; i++) idt_set_gate(i, (uint32_t)default_handler, 0x08, 0x8E);

    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    __asm__ volatile("lidt %0" : : "m"(idtp));
}