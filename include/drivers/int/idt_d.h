#ifndef _DRIVERS_INT_IDT_SYNTHCORE_H
#define _DRIVERS_INT_IDT_SYNTHCORE_H

#include <stdint.h>

#define IDT_SIZE    256

struct IDT_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t reserved;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct IDT_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern struct IDT_entry idt[IDT_SIZE];
extern struct IDT_ptr idtp;

void idt_set_gate(uint8_t vector, uint32_t handler, uint8_t selector, uint8_t flags);

void idt_init();

#endif // _DRIVERS_INT_IDT_SYNTHCORE_H