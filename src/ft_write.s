section .text
    global ft_write
    extern __errno_location

ft_write:
    ; Input: rdi = outfd
    ;        rsi = buffer
    ;        rdx = buflen
    ; Output: rax = number of bytes written or -1 on error

    ; Call the sys_write
    mov rax, 1
    syscall

    ; Check for errors
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

