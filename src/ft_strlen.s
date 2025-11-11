section .text
    global ft_strlen

ft_strlen:
    ; Input: rdi = pointer to null-terminated string
    ; Output: rax = length of the string
    xor rax, rax            ; Clear rax to use it as a counter

    test rdi, rdi           ; Check if the pointer is NULL
    jz .done                ; If NULL, length is 0
.loop:
    cmp byte [rdi + rax], 0 ; Compare current byte with null terminator
    je .done                ; If null terminator found, jump to done
    inc rax                 ; Increment length counter
    jmp .loop               ; Repeat the loop
.done:
    ret                     ; Return with length in rax