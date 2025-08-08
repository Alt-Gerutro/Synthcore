#include <drivers/int/gdt_d.h>
#include <stdint.h>

uint64_t gdt[GDT_SIZE];

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flags) {
    uint64_t descriptor;

    descriptor = limit & 0x000F0000;
    descriptor |= (flags << 8) & 0x00F0FF00;
    descriptor |= (base >> 16) & 0x000000FF;
    descriptor |= base & 0xFF000000;

    descriptor <<= 32;
    descriptor |= base << 16;
    descriptor |= limit & 0x0000FFFF;

    return descriptor;
}

void gdt_init() {
    gdt[GDT_SELECTOR_NULL] = create_descriptor(0, 0, 0);                            /* Null descriptor */
    gdt[GDT_SELECTOR_R0_CODE] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_R0));    /* R0 Code descriptor */
    gdt[GDT_SELECTOR_R0_DATA] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_R0));    /* R0 Data descriptor */
    gdt[GDT_SELECTOR_R3_CODE] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_R3));    /* R3 Code descriptor */
    gdt[GDT_SELECTOR_R3_DATA] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_R3));    /* R3 Data descriptor */

    struct gdtp {
        uint16_t size;
        uint32_t offset;
    } __attribute__((packed)) gdtr = {
        sizeof(gdt) - 1, (uint32_t)&gdt
    };

    __asm__ volatile("lgdt %0" : : "m"(gdtr));

    __asm__ volatile(
        "ljmp %0, $reload_cs\n"
        "reload_cs:\n"
        : : "i"(KERNEL_CS)
    );

    __asm__ volatile("movl %0, %%ds" : : "r"(KERNEL_DS));
    __asm__ volatile("movl %0, %%ss" : : "r"(KERNEL_DS));
    __asm__ volatile("movl %0, %%es" : : "r"(KERNEL_DS));
    __asm__ volatile("movl %0, %%fs" : : "r"(KERNEL_DS));
    __asm__ volatile("movl %0, %%gs" : : "r"(KERNEL_DS));
}