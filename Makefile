# Rules
#• You must write 64-bit assembly. Beware of the "calling convention".
#• You can't do inline ASM, you must do '.s' files.	
#• You must compile your assembly code with nasm.
#• You must use the Intel syntax, not the AT&T syntax.
#It is forbidden to use the compilation flag: -no-pie.

NAME = libasm.a
SRCDIR = src
OBJDIR = obj
SRCS = $(SRCDIR)/ft_strlen.s \
	   $(SRCDIR)/ft_strcpy.s \
	   $(SRCDIR)/ft_strcmp.s \
	   $(SRCDIR)/ft_write.s \
	   $(SRCDIR)/ft_read.s \
	   $(SRCDIR)/ft_strdup.s
OBJS = $(SRCS:$(SRCDIR)/%.s=$(OBJDIR)/%.o)
NASMFLAGS = -f elf64
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.s | $(OBJDIR)
	nasm $(NASMFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) test_program

re: fclean all

test: all
	@gcc -Wall -Wextra -Werror main.c -L. -lasm -o test_program
	@./test_program

valgrind: all
	@gcc -Wall -Wextra -Werror main.c -L. -lasm -o test_program
	@valgrind --leak-check=full ./test_program

.PHONY: all clean fclean re test valgrind