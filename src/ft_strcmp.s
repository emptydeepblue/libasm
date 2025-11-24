section .text
    global ft_strcmp

ft_strcmp:
    ; Input: rdi = s1
    ;        rsi = s2
    ; Output: rax = 0 if s1 == s2, positive or negative value if not

    test rdi, rdi
    jz .handle_null
    test rsi, rsi
    jz .handle_null

.loop:
    mov al, [rdi]                       ; Load byte from s1
    mov bl, [rsi]                       ; Load byte from s2
    cmp al, bl                          ; Compare bytes at current position
    jne .not_equal                      ; If not equal, jump to not_equal
    test al, al                         ; Check for null terminator
    jz .equal                           ; If null terminator, strings are equal
    inc rdi                             ; Move to next byte in s1
    inc rsi                             ; Move to next byte in s2
    jmp .loop                           ; Repeat the loop

.not_equal:
    movzx rax, al
    movzx rbx, bl
    sub rax, rbx
    ret

.equal:
    xor rax, rax                        ; Set rax to 0 for equal strings
    ret                                 ; Return with result in rax

.handle_null:
   mov rax, rdi
   sub rax, rsi
   ret