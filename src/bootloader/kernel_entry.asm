[bits 32]       ; 32-битный код.
[extern kmain]  ; Определить внешнее значение kmain.
[extern kpanic] ; Определить внешнее значение kpanic.

start:
    cli

    mov esp, stack_top
    mov ebp, 0

    call kmain
    call kpanic

    jmp lp

lp:
    jmp $

section .bss
stack_bottom:
    resb 16384 ; Резерв 16 kB
stack_top:
