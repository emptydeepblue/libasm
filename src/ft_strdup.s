section .text
    global ft_strdup
    extern malloc
    extern ft_strlen
    extern ft_strcpy

ft_strdup:
    ; Input rdi = source
    ; Output rdx = duplicate of the source

    test rdi, rdi
    jz .return_null


    ; Calculate length of source string
    call ft_strlen        ; rax = length of source string  
    inc rax               ; Include space for null terminator

    
    ; Allocate memory for duplicate string
    push rdi                        ; Save source pointer
    mov rdi, rax                    ; rdi = size to allocate
    call malloc wrt ..plt           ; Input rdi = size
    
    test rax, rax
    jz .malloc_failed             ; If malloc failed, return NULL

    ; Copy source string to allocated memory
    mov rdi, rax              ; rdi = destination (allocated memory)
    pop rsi                   ; rsi = source string
    call ft_strcpy            ; Copy string

    ret

.malloc_failed:
    pop rdi                   ; Restore source pointer
    ; Fall through to return NULL

.return_null:
    xor rax, rax                ; Return NULL
    ret