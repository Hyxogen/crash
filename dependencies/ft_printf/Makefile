SRC=\
	ft_printf.c \
	ft_printf_parse.c \
	ft_printf_string.c \
	ft_printf_integer.c \
	ft_printf_utils.c \
	ft_printf_superfluous.c
INC=\
	ft_printf.h
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -Wextra -Werror -ansi -pedantic
NAME=libftprintf.a

$(NAME): $(INC) $(OBJ) $(NAME)($(OBJ))
re: fclean all
all: $(NAME)
bonus: $(NAME)

$(NAME)(%.o): %.o
	ar rcs $(NAME) $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

%.o: %.c $(INC)
	$(CC) -c $(CFLAGS) -o $@ $<

norm:
	norminette $(SRC) $(INC)

test: bonus
	if [ ! -d ~/goinfre/test42 ]; then git clone https://github.com/DutChen18/test42 ~/goinfre/test42; fi
	(cd ~/goinfre/test42 && ./test.py test ft_printf $(shell pwd)/libftprintf.a)
