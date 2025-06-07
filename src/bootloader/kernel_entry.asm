[bits 32]       ; 32-битный код.
[extern kmain]  ; Определить внешнее значение kmain.
[extern kpanic] ; Определить внешнее значение kpanic.

cli

mov esp, 0x90000
mov ebp, 0

call kmain
call kpanic

jmp $
