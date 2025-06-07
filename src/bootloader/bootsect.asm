[org 0x7C00]
[bits 16]

KERNEL_OFFSET equ 0x1000

start:
    mov [BOOT_DRIVE], dl      ; Сохраняем номер диска
    mov sp, 0x9000            ; Устанавливаем стек

    ; Загрузка ядра
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov dl, [BOOT_DRIVE]
    mov dh, 16                ; Секторов для чтения
    mov bx, KERNEL_OFFSET     ; Адрес в памяти
    call disk_load

    ; Переход в PM
    call switch_to_pm
    jmp $

%include "print.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "switch.asm"

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PM
    call print_string_pm
    call KERNEL_OFFSET        ; Переход к ядру
    jmp $

; Данные
BOOT_DRIVE:         db 0
MSG_LOAD_KERNEL:    db "Loading kernel...", 0
MSG_PM:             db "Protected Mode", 0

times 510 - ($-$$) db 0
dw 0xAA55
