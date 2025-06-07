[bits 16]
disk_load:
    pusha
    push dx

    mov ah, 0x02
    mov al, dh          ; Секторы для чтения
    mov ch, 0x00        ; Цилиндр 0
    mov dh, 0x00        ; Головка 0
    mov cl, 0x02        ; Начинаем со 2-го сектора

    int 0x13
    jc .error           ; Ошибка если CF=1

    pop dx
    cmp al, dh          ; Проверка количества секторов
    jne .sectors_error
    popa
    ret

.error:
    mov bx, DISK_ERROR
    call print_string
    jmp $

.sectors_error:
    mov bx, SECTORS_ERROR
    call print_string
    jmp $

DISK_ERROR:     db "Disk error!", 0
SECTORS_ERROR:  db "Sector count error!", 0
