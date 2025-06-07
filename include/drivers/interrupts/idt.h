#ifndef _IDT_SYNTHCORE
#define _IDT_SYNTHCORE

#include <utils/types.h>

struct idt_entry {
    u16 offset_low;
    u16 selector;
    u8 zero;
    u8 attrs;
    u16 offset_high;
} __attribute__((packed));

struct idt_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

static inline void lidt(void *base, u16 size) {
    struct idt_ptr ptr = {size, (u32)base};
    asm volatile ("lidt %0" : : "m"(ptr));
}

static inline void sti() {
    asm volatile ("sti");
}

extern struct idt_entry idt[256];
extern struct idt_ptr idtp;

void idt_set_gate(u8 num, void *isr, u16 sel, u8 flags);
void idt_init();

#endif // _IDT_SYNTHCORE
