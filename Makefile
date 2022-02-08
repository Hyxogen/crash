NAME			:= minishell

FILE_NAMES		:= main.c readchar.c readline.c tokenize.c tokenize_op.c tokenize_util.c assert.c memory.c

CC				:= cc
LINK_CMD		:= cc
CFLAGS			:= -Wall -Wextra -pedantic
LFLAGS			:= -Wall -Wextra

SRC_DIR			:= src
LIB_DIR			:= dependencies
INC_DIR			:= include $(LIB_DIR)/libft
OBJ_DIR			:= build
DEP_DIR			:= build

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
$(NAME): $(OBJECTS)
	$(LINK_CMD) -o $@ $(OBJECTS) $(LIB_DIR)/libft/libft.a $(LFLAGS) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -MMD $(patsubst %,-I%,$(INC_DIR))

crash: $(NAME)
	ln -s $(NAME) $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPENDS)
.PHONY: all clean fclean re