# libasm

Recreate basic libc string and I/O functions in x86-64 assembly (Intel syntax, NASM) to learn calling conventions, register usage, and direct syscalls.

**Specialization:** Unix/Kernel
**XP:** 966 · **People:** 1 · **Status:** ✅ Validated

---

## 🚀 Features

- 6 libc-equivalent functions written in pure x86-64 assembly
- Intel syntax with NASM, PIE-compatible objects (no `-no-pie`)
- System V AMD64 calling convention
- Direct Linux syscalls for `read` / `write` with proper `errno` propagation
- Test suites: behavior parity, edge cases, and stress

## 🛠️ Build & Run

**Prerequisites:** NASM, GCC, GNU Make, Linux x86_64

```bash
make           # Build libasm.a
make demo      # Build a demo binary against libasm.a and run it
make valdemo   # Same demo, under valgrind
make clean     # Remove objects
make fclean    # Remove library + objects
```

Link with: `gcc your_prog.c libasm.a -o your_prog`

## 🧪 Testing

```bash
make test-base     # Behavior parity vs. libc
make test-eval     # Eval-style edge cases
make test-stress   # Random / stress inputs
```

## 📁 Project Structure

```
libasm/
├── src/                # ft_strlen.s, ft_strcpy.s, ft_strcmp.s,
│                       # ft_write.s, ft_read.s, ft_strdup.s
├── test/               # base / eval / stress test suites
├── docs/               # Notes
├── main.c              # Demo entry point
└── Makefile
```

## 🔧 Implementation

**Functions:**

| Function     | Prototype                                              |
|--------------|--------------------------------------------------------|
| `ft_strlen`  | `size_t ft_strlen(const char *s)`                      |
| `ft_strcpy`  | `char *ft_strcpy(char *dst, const char *src)`          |
| `ft_strcmp`  | `int ft_strcmp(const char *s1, const char *s2)`        |
| `ft_write`   | `ssize_t ft_write(int fd, const void *buf, size_t)`    |
| `ft_read`    | `ssize_t ft_read(int fd, void *buf, size_t)`           |
| `ft_strdup`  | `char *ft_strdup(const char *s)`                       |

**Technical details:**
- System V AMD64 ABI: args in `RDI`, `RSI`, `RDX`, `RCX`, `R8`, `R9`; return in `RAX`
- Syscalls invoked via the `syscall` instruction; on error, errno is set through libc's `__errno_location` (called via PLT for PIE)
- 16-byte stack alignment maintained across external calls (`malloc`, `__errno_location`, …)
- `ft_strdup` composes `ft_strlen`, `malloc`, and `ft_strcpy`

## 📚 Learning Objectives

- x86-64 register usage and stack discipline
- Intel vs. AT&T syntax, NASM toolchain
- System V AMD64 calling convention
- Direct Linux syscalls and errno propagation
- Position-independent code and the PLT

## 🔗 References

- [System V AMD64 ABI](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)
- `man 2 syscall`, `man 2 read`, `man 2 write`
- [NASM Manual](https://www.nasm.us/doc/)

---

*42 School — RNCP7 · Unix/Kernel*
