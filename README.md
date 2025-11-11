# 42 libasm

A 42 School project implementing C library functions in x86-64 assembly language.

## 📋 Overview

Recreate basic C functions using Intel syntax assembly to learn low-level programming and CPU architecture.

## 🎯 Requirements

- **Architecture**: x86-64 (64-bit)
- **Syntax**: Intel (not AT&T)
- **Assembler**: NASM
- **Files**: `.s` extensions only
- **Forbidden**: `-no-pie` flag

## 📚 Functions

- `ft_strlen` - String length
- `ft_strcpy` - Copy string  
- `ft_strcmp` - Compare strings
- `ft_write` - Write to file descriptor
- `ft_read` - Read from file descriptor
- `ft_strdup` - Duplicate string

## 🛠️ Usage

```bash
make          # Build libasm.a
make test     # Run tests
make clean    # Remove objects
make fclean   # Remove all
```

## 📖 Prototypes

```c
size_t  ft_strlen(const char *s);
char    *ft_strcpy(char *dst, const char *src);
int     ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fd, const void *buf, size_t count);
ssize_t ft_read(int fd, void *buf, size_t count);
char    *ft_strdup(const char *s);
```

## 🔧 Key Concepts

- **Calling Convention**: System V ABI
- **Registers**: RAX (return), RDI (arg1), RSI (arg2), RDX (arg3)
- **System Calls**: read(0), write(1)
- **Error Handling**: errno, return codes

---

*42 School Assembly Project*