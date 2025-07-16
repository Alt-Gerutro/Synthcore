.global _start
.extern stack_top
.extern kmain
.extern kpanic

.section .multiboot
.align 4
.set MULTIBOOT_MAGIC, 0x1BADB002
.set MULTIBOOT_FLAGS, 0
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM


.section .text
_start:
    mov $stack_top, %esp
    call kmain
    call kpanic
.hang:
    hlt
    jmp .hang