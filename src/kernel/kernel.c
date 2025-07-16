#include <config/kernel/kernel_config.h>
#include <utils/types.h>
#include <drivers/io/base_io.h>
#include <drivers/interrupts/pic.h>
#include <drivers/interrupts/idt.h>

extern void _start(void);

__attribute__((noreturn))
void kmain() {
    clear_screen();

    pic_remap();
    keyboard_init();

    while(1) { asm volatile ("hlt"); }
}

__attribute__((noreturn))
void kpanic() {
    print_str("KERNEL PANIC. STOPPED.", 0x40);
    asm volatile ("cli\nhlt");
    __builtin_unreachable();
}
