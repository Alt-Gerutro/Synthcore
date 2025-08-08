#include <config/kernel/kernel_config.h>
#include <stdint.h>
#include <drivers/int/interrupts_h.h>
#include <drivers/io/screen/screen_d.h>
#include <drivers/io/input/input_d.h>

__attribute__((section(".multiboot"), used))
static const struct {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
} multiboot_header = { MULTIBOOT_MAGIC, MULTIBOOT_FLAGS, MULTIBOOT_CHECKSUM };

__attribute__((noreturn, used))
void kpanic() {
    print_str("KERNEL PANIC\n", make_color(VGA_BYTE_DARK_RED, VGA_BYTE_BLACK));

    while (1) {
        __asm__ volatile("cli");
        __asm__ volatile("hlt");
    }
}

__attribute__((noreturn, used))
void kmain() {
    int_init();
    irq_mask(0);
    clear_screen();

    print_str("Hello, World!\n", make_color(VGA_BYTE_BLACK, VGA_BYTE_BLUE));

    keyboard_init();

    while (1) {
        __asm__ volatile ("hlt");
    }
}