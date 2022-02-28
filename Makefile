NAME			:= minishell

BASE_FILES		:= \
	debug.c memory.c assert.c op.c
LEXER_FILES		:= \
	expand.c init.c add.c lex.c operator.c escape.c advance.c main.c \
	special.c source.c
INPUT_FILES		:= \
	input_file.c input_readline.c input_string.c input.c
PARSER_FILES	:= \
	and_or.c case.c command_extra.c command.c condition.c convert.c \
	function.c general.c io.c list.c loop.c node_int.c node.c other.c \
	pipe.c redirect.c separator.c subshell.c

FILE_NAMES		:= \
	$(BASE_FILES) \
	$(patsubst %,lexer/%,$(LEXER_FILES)) \
	$(patsubst %,input/%,$(INPUT_FILES)) \
	$(patsubst %,parser/%,$(PARSER_FILES))

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
	CFLAGS		+= -DSH_DEBUG=1 -fsanitize=address,undefined -g3 -Og
	LFLAGS		+= -DSH_DEBUG=1 -fsanitize=address,undefined
else ifeq ($(config), release)
	CFLAGS		+= -Werror -g3 -O2
	LFLAGS		+= -Werror
else ifeq ($(config), distr)
	CFLAGS		+= -Werror -g0 -Ofast
	LFLAGS		+= -Werror 
else
$(error "invalid config $(config"))
endif

all: $(NAME) crash

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
	rm -rf build
	${MAKE} -C $(LIBFT_DIR) fclean

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPENDS)
.PHONY: all clean fclean re