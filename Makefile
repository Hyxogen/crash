NAME			:= minishell

FILE_NAMES		:= main.c memory.c assert.c new_lexer/expand.c new_lexer/init.c new_lexer/add.c \
				new_lexer/lex.c new_lexer/operator.c new_lexer/escape.c new_lexer/advance.c \
				new_lexer/main.c new_lexer/debug.c new_lexer/special.c new_lexer/source.c \
				new_input/input_file.c new_input/input_readline.c new_input/input_string.c \
				new_input/input.c parser/parser.c parser/convert.c parser/debug.c op.c

CC				:= cc
LINK_CMD		:= cc
CFLAGS			:= -Wall -Wextra -pedantic
LFLAGS			:= -Wall -Wextra

SRC_DIR			:= src
LIB_DIR			:= dependencies
INC_DIR			:= include $(LIB_DIR)/libft
OBJ_DIR			:= build
DEP_DIR			:= build

LIBFT_DIR			:= $(LIB_DIR)/libft
LIBFT_LIB			:= $(LIBFT_DIR)/libft.a

SOURCES			:= $(patsubst %.c,$(SRC_DIR)/%.c,$(FILE_NAMES))
OBJECTS			:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILE_NAMES))
DEPENDS			:= $(patsubst %.c,$(DEP_DIR)/%.d,$(FILE_NAMES))

ifndef config
	config = debug
endif

ifeq ($(config), debug)
	CFLAGS		+= -DSH_DEBUG=1 -fsanitize=address -g3 -Og
	LFLAGS		+= -DSH_DEBUG=1 -fsanitize=address
else ifeq ($(config), release)
	CFLAGS		+= -Werror -g3 -O2
	LFLAGS		+= -Werror
else ifeq ($(config), distr)
	CFLAGS		+= -Werror -g0 -Ofast
	LFLAGS		+= -Werror 
else
$(error "invalid config $(config"))
endif

all: $(NAME)

# TODO compile libft using its own makefile
$(NAME): $(OBJECTS) $(LIBFT_LIB)
	$(LINK_CMD) -o $@ $(OBJECTS) $(LIBFT_LIB) $(LFLAGS) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $< -MMD $(patsubst %,-I%,$(INC_DIR))

$(LIBFT_LIB):
	${MAKE} -C $(LIBFT_DIR) bonus

crash: $(NAME)
	ln -s $(NAME) $@

clean:
	rm -f $(OBJECTS)
	${MAKE} -C $(LIBFT_DIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPENDS)
.PHONY: all clean fclean re