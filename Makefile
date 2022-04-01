NAME			:= minishell

BASE_FILES		:=

ifndef debug
	BASE_FILES	+= \
		main.c
else
	BASE_FILES	+= \
		debug.c
endif

LEXER_FILES		:= \
	expand.c init.c add.c lex.c operator.c escape.c advance.c main.c \
	special.c source.c source_extra.c die.c
INPUT_FILES		:= \
	input_file.c input_readline.c input_string.c input.c
PARSER_FILES	:= \
	and_or.c case.c command_extra.c command.c condition.c convert.c \
	function.c general.c io.c list.c loop.c node_int.c node.c other.c \
	pipe.c redirect.c separator.c subshell.c die.c error.c global.c
COMMANDER_FILES	:= \
	commander.c execvp.c pipe_sequence.c env.c redirect_process.c expand.c \
	signal.c expand_param.c expand_command.c expand_pattern.c echo.c
UTIL_FILES		:= \
	die.c memory.c op.c util.c wrap.c

FILE_NAMES		:= \
	$(BASE_FILES) \
	$(patsubst %,lexer/%,$(LEXER_FILES)) \
	$(patsubst %,input/%,$(INPUT_FILES)) \
	$(patsubst %,parser/%,$(PARSER_FILES)) \
	$(patsubst %,commander/%,$(COMMANDER_FILES)) \
	$(patsubst %,util/%,$(UTIL_FILES))

CC				:= cc
LINK_CMD		:= $(CC)
CFLAGS			:= -Wall -Wextra -pedantic
LFLAGS			:= -Wall -Wextra

SRC_DIR			:= src
LIB_DIR			:= dependencies
OBJ_DIR			:= build
DEP_DIR			:= build

LIBFT_DIR		:= $(LIB_DIR)/libft
LIBFT_LIB		:= $(LIBFT_DIR)/libft.a

INC_DIR			:= include $(LIBFT_DIR)

SOURCES			:= $(patsubst %.c,$(SRC_DIR)/%.c,$(FILE_NAMES))
OBJECTS			:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILE_NAMES))
DEPENDS			:= $(patsubst %.c,$(DEP_DIR)/%.d,$(FILE_NAMES))

BLACK					:="\033[0;30m"
RED						:="\033[0;31m"
GREEN					:="\033[0;32m"
ORANGE					:="\033[0;33m"
BLUE					:="\033[0;34m"
PURPLE					:="\033[0;35m"
CYAN					:="\033[0;36m"
LIGHT_GRAY				:="\033[0;37m"

DARK_GRAY				:="\033[1;30m"
LIGHT_RED				:="\033[1;31m"
LIGHT_GREEN				:="\033[1;32m"
YELLOW					:="\033[1;33m"
LIGHT_BLUE				:="\033[1;34m"
LIGHT_PURPLE			:="\033[1;35m"
LIGHT_CYAN				:="\033[1;36m"
WHITE					:="\033[1;37m"

RESET					:="\033[0m"

COMPILE_COLOR			:= $(GREEN)
LINK_COLOR				:= $(CYAN)
OBJECT_COLOR			:= $(RED)

ifndef config
	config = debug
endif

ifeq ($(config), debug)
	CFLAGS		+= -DSH_DEBUG=1 -g3 -Og
	LFLAGS		+= -DSH_DEBUG=1
	ifndef nsan
		CFLAGS	+= -fsanitize=address,undefined
		LFLAGS	+= -fsanitize=address,undefined
	endif
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

test:
	make CC=clang
	ASAN_OPTIONS=detect_leaks=1 ./crash < tests/scripts/bf.sh
	ASAN_OPTIONS=detect_leaks=1 ./crash < tests/scripts/cowsay.sh
	ASAN_OPTIONS=detect_leaks=1 ./crash < tests/scripts/fib.sh
	ASAN_OPTIONS=detect_leaks=1 ./crash < tests/scripts/tableflip.sh

-include $(DEPENDS)
.PHONY: all clean fclean re