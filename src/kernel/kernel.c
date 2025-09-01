#include <config/kernel/kernel_config.h>
#include <arch/x86/interrupts/interrupts.h>
#include <drivers/video/screen.h>
#include <drivers/keyboard/input.h>
#include <stdint.h>

MULTIBOOT_HEADER;

__attribute__((noreturn, used))
void kpanic(char* msg, ...) {
    char buf[2048];
    va_list args;
    va_start(args, msg);
    vformat(buf, 2048, msg, args);
    va_end(args);
    print_str(buf, make_color(VGA_BYTE_DARK_RED, VGA_BYTE_BLACK));

    __asm__ volatile("cli");
    while (1) {
        __asm__ volatile("hlt");
    }
}

__attribute__((noreturn, used))
void kmain() {
    int_init();
    irq_mask(0);
    clear_screen();

    print_fmt("Hello %s. %d\n", "World!", 623);
    kpanic("[Test] CRITICAL ERROR: %s. %d\n", "yes.", 123123);
    
    keyboard_init();

    while (1) {
        __asm__ volatile ("hlt");
    }
}