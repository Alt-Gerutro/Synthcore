;;;;;;;;;;;;;;;;;;;;;;;
;;    P. STR PART    ;;
;;;;;;;;;;;;;;;;;;;;;;;

[bits 16]

print_string:                   ; Функция вывода строки на экран (RM).
    pusha                       ; Запись всех значений регистров в стек.

    mov ah, 0x0E                ;

    ps_loop:
        mov al, [bx]            ;

        cmp al, 0               ; Проверка на конец строки.
        je new_line             ; Если конец строки то перейти к метке new_line
        jmp put_char            ; Иначе перейти к метке put_char.

put_char:
    int 0x10                    ; Прерывание.
    inc bx                      ; Увеличение bx на 1.
    jmp ps_loop                 ; Перейти к метке ps_loop.

new_line:
    mov ah, 0x0E                ;
    mov al, 0x0A                ; Перенос строки.
    int 0x10                    ; Прерывание.
    mov al, 0x0D                ; Возврат курсора.
    int 0x10                    ; Прерывание.
    jmp return                  ; Перейти к метке return.

return:
    popa                        ; Возвращение регистрам значений из стека.
    ret                         ; Завершение выполнения.


;;;;;;;;;;;;;;;;;;;;;;;
;;    P. HEX PART    ;;
;;;;;;;;;;;;;;;;;;;;;;;


print_hex:
    pusha                       ; Запись всех значений регистров в стек.
    mov cx, 0                   ; Инициализация счётчика в регистре cx.

ph_loop:
    cmp cx, 4                   ; Сравнить счётчик с 4. (cx < 4).
    jl hex_print                ; Перейти к метке hex_print,
                                ; если флаг знака не равен флагу переполнения.
    jmp hex_end                 ; Перейти к метке ph_loop.

hex_print:
    mov ax, dx                  ; Скопировать dx в ax
    and ax, 0x000F
    cmp ax, 9                   ;

    jg num_to_abc               ;
    jmp hex_next                ; Перейти к метке hex_next.

num_to_abc:
    add ax, 39                  ;
    jmp hex_next                ; Перейти к метке hex_next.

hex_next:
    add ax, '0'                 ;
    mov bx, HEX_OUT + 5         ;

    sub bx, cx                  ;

    mov [bx], al                ;

    ror dx, 4                   ;
    inc cx                      ; Увеличение cx на 1.
    jmp ph_loop                 ; Перейти к метке ph_loop.

hex_end:
    mov bx, HEX_OUT             ; Передаём строку адреса в bx.
    call print_string           ; Вызов функции print_string из секции `P. STR PART`.
    popa                        ; Возвращение регистрам значений из стека.
    ret                         ; Завершение выполнения.

HEX_OUT:    db "0x0000", 0      ;


;;;;;;;;;;;;;;;;;;;;;;;
;; P. STR IN PM PART ;;
;;;;;;;;;;;;;;;;;;;;;;;


[bits 32]                       ;

print_string_pm:                ; Запись всех значений регистров в стек.
    pusha                       ;
    mov edx, 0xB8000

print_string_pm_loop:
    mov al, [ebx]               ;
    mov ah, 0x0F                ;

    cmp al, 0                   ;
    je print_string_pm_end      ;

    mov [edx], ax               ;
    add ebx, 1                  ;
    add edx, 2                  ;

    jmp print_string_pm_loop    ;

print_string_pm_end:
    popa                        ; Возвращение регистрам значений из стека.
    ret                         ; Завершение выполнения.
