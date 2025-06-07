#include <drivers/interrupts/idt.h>
//#include <drivers/io/base_io.h>

struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(u8 num, void *isr, u16 sel, u8 flags) {
    //print_str("Calling idt_set_gate...\n", 0x07);
    idt[num].offset_low = (u32)isr & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].attrs = flags;
    idt[num].offset_high = ((u32)isr >> 16) & 0xFFFF;
}

__attribute__((naked)) static void default_isr() {
    asm volatile (
        "pusha\n\t"
        "movb $0x20, %al\n\t"  // EOI для Master PIC
        "outb %al, $0x20\n\t"
        "popa\n\t"
        "iret"
    );
}

void idt_init() {
    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, default_isr, 0x08, 0x8E);
    }
    //print_str("IDT initialized.\n", 0x07);

    // lidt(idt, sizeof(idt) - 1);
}
