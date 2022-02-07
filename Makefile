NAME			:= minishell

FILE_NAMES		:= main.c readchar.c readline.c

CC				:= clang
LINK_CMD		:= clang
CFLAGS			:=
LFLAGS			:= -Wall -Wextra -Werror

SRC_DIR			:= src
LIB_DIR			:= dependencies
INC_DIR			:= include $(LIB_DIR)/libft/include
OBJ_DIR			:= build
DEP_DIR			:= build

SOURCES			:= $(patsubst %.c,$(SRC_DIR)/%.c,$(FILE_NAMES))
OBJECTS			:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(FILE_NAMES))
DEPENDS			:= $(patsubst %.c,$(DEP_DIR)/%.d,$(FILE_NAMES))

ifndef config
	config = debug
endif

ifeq ($(config), debug)
	CFLAGS		+= -fsanitize=address,undefined -g3 -Og
	LFLAGS		+= -fsanitize=address,undefined
else ifeq ($(config), release)
	CFLAGS		+= -g3 -O2
	LFLAGS		+=
else ifeq ($(config), distr)
	CFLAGS		+= -g0 -Ofast
	LFLAGS		+= 
else
$(error "invalid config $(config"))
endif

all: $(NAME)

$(NAME): $(OBJECTS)
	$(LINK_CMD) -o $@ $(OBJECTS) $(LFLAGS) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -I$(INC_DIR)

crash: $(NAME)
	ln -s $(NAME) $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPENDS)
.PHONY: all clean fclean re