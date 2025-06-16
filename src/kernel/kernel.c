#include <kernel_config.h>
#include <utils/types.h>
#include <drivers/io/base_io.h>
#include <drivers/interrupts/pic.h>
#include <drivers/interrupts/idt.h>

__attribute__((noreturn))
__attribute__((used))
void kmain() {
    clear_screen();

    pic_remap();
    keyboard_init();

    while(1) { asm volatile ("hlt"); }
}

__attribute__((used))
void kpanic() {
    print_str("KERNEL PANIC. STOPPED.", 0x40);
    asm volatile ("cli\n\t"
        "hlt\n\t");
}
