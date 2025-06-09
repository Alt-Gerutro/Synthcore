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

    print_str("Hello", 0x0D);
    put_char('a', 0x0E);
    put_char('\n', 0x0A);
    put_char(get_char(1), get_attr(6));
    put_char('\n', 0x0F);

    while(1) { asm volatile ("hlt"); }
}

__attribute__((used))
void kpanic() {
    print_str("KERNEL PANIC. STOPPED.", 0x40);
    asm volatile ("cli\n\t"
        "hlt\n\t");
}
