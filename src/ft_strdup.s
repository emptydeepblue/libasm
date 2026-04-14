section .text
    global ft_strdup
    extern malloc
    extern ft_strlen
    extern ft_strcpy

ft_strdup:
    ; Input:  rdi = source string
    ; Output: rax = duplicate string, or NULL on failure
    ;
    ; Stack layout at entry: rsp % 16 = 8  (SysV ABI: call pushed return addr)
    ; push r12 makes rsp % 16 = 0, so every subsequent call sees rsp % 16 = 8 ✓
    ; r12 is callee-saved — we use it to hold the source pointer across calls.

    push r12                        ; align stack + save r12 (callee-saved)

    test rdi, rdi
    jz .return_null

    mov r12, rdi                    ; save source pointer

    ; --- ft_strlen(r12) → rax = length ---
    call ft_strlen                  ; rsp % 16 = 8 at callee entry ✓
    inc rax                         ; +1 for null terminator

    ; --- malloc(rax) → rax = allocated buffer ---
    mov rdi, rax
    call malloc wrt ..plt           ; rsp % 16 = 8 at callee entry ✓

    test rax, rax
    jz .return_null                 ; malloc failed → return NULL

    ; --- ft_strcpy(rax, r12) → rax = dst ---
    mov rdi, rax                    ; dst = allocated buffer
    mov rsi, r12                    ; src = saved source pointer
    call ft_strcpy                  ; rsp % 16 = 8 at callee entry ✓
                                    ; ft_strcpy returns dst in rax ✓

    pop r12
    ret

.return_null:
    xor rax, rax
    pop r12
    ret