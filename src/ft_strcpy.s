section .text
    global ft_strcpy

ft_strcpy:
    ; Input: rdi = destination pointer
    ;        rsi = source pointer
    ; Output: rax = destination pointer (same as rdi)
    mov rax, rdi            ; Save destination pointer to return later

    test rsi, rsi
    jz .done

.loop:
    mov bl, [rsi]           ; Load byte from source
    mov [rdi], bl           ; Store byte to destination

    inc rdi                 ; Move to next byte in destination
    inc rsi                 ; Move to next byte in source

    cmp bl, 0               ; Check if it was the null terminator
    jne .loop
.done:
    ret