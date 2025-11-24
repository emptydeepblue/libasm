section .text
    global ft_read
    extern __errno_location

ft_read:
    ; Input rdi = infd
    ;       rsi = buffer
    ;       rdx = buflen
    ; Output: rax = number of bytes readed oor -1 on error

    ; call the sys_read
    mov rax, 0
    syscall

    test rax, rax
    js .error

    ret

.error:
    ; Set errno and return -1
    neg rax             ; get absolute code
    push rax            ; save error code
    call __errno_location wrt ..plt ; Call with PLT for PIE compatibility
    pop rdx             ; restore error code
    mov [rax], rdx      ; set errno
    mov rax, -1         ; return -1    
    ret

