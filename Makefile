NAME			:= minishell

BASE_FILES		:=

ifndef debug
	BASE_FILES	+= \
		main.c history.c
else
	BASE_FILES	+= \
		debug.c history.c
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
	commander.c execvp.c pipe_sequence.c env.c redirect_process1.c signal.c \
	redirect_builtin1.c redirect_builtin2.c run.c condition.c loop.c \
	command.c expansion.c expand1.c expand_param1.c expand_command.c \
	expand_arith.c new_pattern.c new_pattern_brackets.c new_pattern_class.c \
	new_pattern_generate.c function.c init.c redirect_general.c and_or_if.c \
	return_code.c assignment.c compound_command.c term.c wildcard.c \
	command_util.c command_internal_exec.c command_redirect.c env_int.c \
	env_util.c expand_special1.c expand_special2.c expand_param_util.c \
	expand_arith_tok.c expand_arith_bin_init1.c expand_arith_bin_init2.c \
	expand_arith_optok_init.c expand_arith_unary.c expand_arith_bin1.c \
	expand_arith_lex.c builtins.c pipe_sequence_rec.c pipe_sequence_command.c \
	wait.c word_list.c expand_arith_bin2.c redirect_process2.c \
	expand_param2.c wildcard2.c expand2.c expand3.c
ARITH_FILES		:= \
	arith_plus.c arith_minus.c arith_modulo.c arith_divide.c arith_multiply.c \
	arith_ternary.c arith_shift.c arith_inequality.c arith_equality.c \
	arith_bitwise.c arith_bitwise_assign.c arith_condition.c arith_unary.c \
	arith_assign.c arith_identity.c
BUILTINS_FILES	:= \
	set.c echo.c dot.c colon.c exit.c break.c export.c continue.c shift.c \
	getopts.c cd.c pwd.c unset.c env.c true.c false.c unimplemented.c
UTIL_FILES		:= \
	die.c memory.c op.c util.c wrap.c strlst1.c err.c atol.c file.c ltoa.c \
	wrap2.c wrap3.c termios.c stringlst1.c basename.c global.c stringlst2.c \
	strlst2.c backtrace.c

FILE_NAMES		:= \
	$(BASE_FILES) \
	$(patsubst %,lexer/%,$(LEXER_FILES)) \
	$(patsubst %,input/%,$(INPUT_FILES)) \
	$(patsubst %,parser/%,$(PARSER_FILES)) \
	$(patsubst %,commander/%,$(COMMANDER_FILES)) \
	$(patsubst %,arith/%,$(ARITH_FILES)) \
	$(patsubst %,builtins/%,$(BUILTINS_FILES)) \
	$(patsubst %,util/%,$(UTIL_FILES))

CC				:= clang
LINK_CMD		:= $(CC)
CFLAGS			:= -Wall -Wextra -pedantic
LFLAGS			:= -Wall -Wextra

SRC_DIR			:= src
LIB_DIR			:= dependencies
OBJ_DIR			:= build
DEP_DIR			:= build

LIBFT_DIR		:= $(LIB_DIR)/libft
LIBFT_LIB		:= $(LIBFT_DIR)/libft.a
FT_PRINTF_DIR	:= $(LIB_DIR)/ft_printf
FT_PRINTF_LIB	:= $(FT_PRINTF_DIR)/libftprintf.a

INC_DIR			:= include $(LIBFT_DIR) $(FT_PRINTF_DIR)

CFLAGS          += -I$(HOME)/.brew/opt/readline/include
LFLAGS          += -L$(HOME)/.brew/opt/readline/lib

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
CLEAN_COLOR				:= $(PURPLE)

ifndef config
	config = distr
endif

ifndef san
	san := address
endif 

ifeq ($(config), debug)
	CFLAGS		+= -DSH_DEBUG=1 -fno-inline -g3 -O0 -DSH_BACKTRACE
	LFLAGS		+= -DSH_DEBUG=1 -fno-inline
	ifeq ($(san), address)
		CFLAGS	+= -fsanitize=address,undefined
		LFLAGS	+= -fsanitize=address,undefined
	else ifeq ($(san), memory)
		CFLAGS	+= -fsanitize=memory,undefined
		LFLAGS	+= -fsanitize=memory,undefined
	endif
else ifeq ($(config), release)
	CFLAGS		+= -g3 -O2
	LFLAGS		+=
else ifeq ($(config), distr)
	CFLAGS		+= -g0 -Ofast -flto
	LFLAGS		+= -g0 -Ofast -flto
else
$(error "invalid config $(config"))
endif

all: $(NAME) crash

SILENT			:=

ifndef verbose
	SILENT		:= @
endif

$(NAME): $(OBJECTS) $(LIBFT_LIB) $(FT_PRINTF_LIB)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(LINK_CMD) -o $@ $(OBJECTS) $(LIBFT_LIB) $(FT_PRINTF_LIB) $(LFLAGS) -L$(HOME)/.brew/Cellar/readline/8.1.2/lib/ -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(SILENT)mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) $(CFLAGS) -c -o $@ $< -MMD $(patsubst %,-I%,$(INC_DIR))

$(LIBFT_LIB):
	$(SILENT)${MAKE} -C $(LIBFT_DIR) bonus

$(FT_PRINTF_LIB):
	$(SILENT)${MAKE} -C $(FT_PRINTF_DIR) bonus

crash: $(NAME)
	$(SILENT)ln -s $(NAME) $@

clean:
	@printf $(CLEAN_COLOR)Cleaning\ object\ files\ and\ dependencies$(RESET)\\n
	$(SILENT)rm -rf build
	$(SILENT)${MAKE} -C $(LIBFT_DIR) fclean
	$(SILENT)${MAKE} -C $(FT_PRINTF_DIR) fclean

fclean: clean
	@printf $(CLEAN_COLOR)Cleaning\ output\ files$(RESET)\\n
	$(SILENT)rm -f $(NAME)
	$(SILENT)rm -f crash

re: fclean
	make all

test:
	make CC=clang
	# ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppressions.txt ./$(NAME) tests/scripts/bf.sh
	ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppressions.txt ./$(NAME) tests/scripts/cowsay.sh "Hello, World!"
	ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppressions.txt ./$(NAME) tests/scripts/fib.sh
	ASAN_OPTIONS=detect_leaks=1 LSAN_OPTIONS=suppressions=suppressions.txt ./$(NAME) tests/scripts/tableflip.sh

-include $(DEPENDS)
.PHONY: all clean fclean re
