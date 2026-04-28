# Assembly Language

Assembly language is a low-level programming language that is closely related to machine code. It provides a way for programmers to write instructions in a human-readable format, which are then translated into machine code by an assembler. Assembly language is specific to a particular computer architecture, meaning that code written for one type of processor may not work on another.

## Basic Concepts

### Instructions
Assembly language consists of a series of instructions that correspond to the operations performed by the CPU. Each instruction typically consists of an operation code (opcode) and operands. The opcode specifies the operation to be performed, while the operands specify the data to be used in the operation.

Here are the main categories of operations:

**Arithmetic Operations**
- **ADD** - Addition
- **SUB** - Subtraction  
- **MUL** - Multiplication
- **DIV** - Division
- **INC** - Increment by 1
- **DEC** - Decrement by 1

**Data Movement Operations**
- **MOV** - Move/copy data between registers, memory, or immediate values
- **LEA** - Load effective address
- **PUSH** - Push data onto stack
- **POP** - Pop data from stack

**Logical Operations**
- **AND** - Bitwise AND
- **OR** - Bitwise OR
- **XOR** - Bitwise XOR
- **NOT** - Bitwise NOT
- **SHL/SHR** - Shift left/right

**Comparison & Control Flow**
- **CMP** - Compare two values
- **JMP** - Unconditional jump
- **JE/JNE** - Jump if equal/not equal
- **CALL** - Call a function
- **RET** - Return from function

*Whether you use registers or memory depends on the specific instruction and what's most efficient for your needs. Most of these operations can work with different combinations of registers, memory locations, and immediate values (constants).*

### Registers
Registers are small, fast storage locations within the CPU that hold data temporarily during execution. Assembly language instructions often involve moving data between registers and memory or performing operations directly on register values.

**Options for Operations:**
While registers are the **fastest** way to perform operations, modern assembly languages allow you to operate directly on memory locations as well.

1. **Register-to-Register** (Fastest)
````assembly
mov eax, 5      ; Load immediate value into register
add eax, ebx    ; Add register to register
````

2. **Register-to-Memory**
````assembly
add eax, [var1]     ; Add memory value to register
mov [result], eax   ; Store register value to memory
````

3. **Memory-to-Memory** (Limited)
````assembly
; This is NOT allowed in x86:
; add [var1], [var2]  ; Can't do memory-to-memory directly

; Instead, you need:
mov eax, [var1]     ; Load first value into register
add eax, [var2]     ; Add second memory value to register
mov [result], eax   ; Store result back to memory
````

**Key Points:**

- **Registers are preferred** because they're much faster than memory access
- **At least one operand** in most arithmetic operations must be a register in x86
- You can operate directly on memory, but with limitations
- The CPU architecture determines what combinations are allowed

### Labels
Labels are used in assembly language to mark specific locations in the code. They serve as reference points for jumps and branches, allowing the program to change its flow of execution based on certain conditions.
### Directives
Directives are special instructions that provide guidance to the assembler. They are not translated into machine code but are used to define data, allocate memory, and control the assembly process.
## Example
Here is a simple example of assembly language code for the x86 architecture:

```assembly
section .data
    msg db 'Hello, World!', 0

section .text
    global _start

_start:
    ; Write the message to stdout
    mov eax, 4          ; syscall: sys_write
    mov ebx, 1          ; file descriptor: stdout
    mov ecx, msg       ; pointer to message
    mov edx, 13        ; message length
    int 0x80           ; call kernel

    ; Exit the program
    mov eax, 1          ; syscall: sys_exit
    xor ebx, ebx        ; exit code: 0
    int 0x80           ; call kernel
```

In this example, the program writes "Hello, World!" to the standard output and then exits. The `section .data` defines a data segment where the message is stored, while the `section .text` contains the executable code. The `_start` label marks the entry point of the program.

## Syscalls
System calls (syscalls) are used to request services from the operating system's kernel. In assembly language, syscalls are typically invoked using specific instructions that trigger a software interrupt or a special CPU instruction.

**Most common syscalls in Linux x86-64:**
| Syscall Number | Function        | Description                     |
|----------------|-----------------|---------------------------------|
| 1              | sys_exit        | Exit a program                  |
| 4              | sys_write       | Write to a file descriptor      |
| 5              | sys_open        | Open a file                     |
| 6              | sys_close       | Close a file descriptor         |
| 60             | sys_exit_group  | Exit all threads in a process   |

**Example of a syscall in x86 Linux:**
```assembly
mov rax, 1          ; syscall: sys_exit
xor rdi, rdi        ; exit code: 0
syscall             ; invoke the syscall
```

## Running Assembly Code
To run assembly code, you typically need to follow these steps:

1. **run an assembler** to convert the assembly code into machine code (object file).
````bash
as program.asm -o program.o
````

2. **run it with gcc** to link the object file and create an executable.
````bash
gcc -o program program.o -nostdlib -static
````

3. **execute the program**.
````bash
./program
````

## Hello World in Assembly
Here is a simple "Hello, World!" program written in x86-64 assembly language for Linux:

```assembly
section .data
    hello_world db "Hello, World!", 10, 0    ; 10 is newline

section .text
    global _start

_start:
    ; sys_write call
    mov rax, 1                  ; syscall operation
    mov rdi, 1                  ; file descriptor for stdout
    mov rsi, hello_world        ; pointer to string
    mov rdx, 14                 ; size of the buffer
    syscall

    ; sys_exit call
    mov rax, 60                 ; syscall operation
    mov rdi, 0                  ; exit value (changed to 0 for success)
    syscall
```

## get parameters
````assembly
section .text
    global ret_param

ret_param:
    ; rdi contains the first argument
    mov rax, rdi      ; Move the argument to rax to return it
    ret               ; Return to caller
````

## ret 42
````assembly
section .text
    global ret_42

ret_42:
    mov rax, 42       ; Move 42 into rax
    ret               ; Return to caller
````

## Testing Assembly Functions in C
To test assembly functions from C, you can create a simple C program that calls the assembly functions and checks their return values. Here's an example of how to set this up: 

```c
// Declare your assembly function
int ret_42(void);
int main(void)
{
    int result = ret_42();
    
    // Exit with the result as exit code
    // You can check with: echo $? after running
    return (result == 42) ? 0 : 1;  // 0 = success, 1 = failure
}
```

## Function arguments
