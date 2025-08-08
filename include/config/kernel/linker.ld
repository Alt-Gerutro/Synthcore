ENTRY(kmain)

STACK_SIZE = 16K;

SECTIONS {
    . = 1M;

    .text : ALIGN(4) {
        *(.multiboot)
        *(.text)
    }

    .rodata : ALIGN(4) {
        *(.rodata)
    }

    .data : ALIGN(4) {
        *(.data)
    }

    .bss : ALIGN(4) {
        *(COMMON)
        *(.bss)
    }

    . = ALIGN(16);
    kernel_stack_bottom = .;
    . += STACK_SIZE;
    kernel_stack_top = .;

    /DISCARD/ : {
        *(.eh_frame)
        *(.comment)
    }
}