TARGET				:= crash_test

CC					:= clang
CXX					:= clang++
LINK_CMD			:= clang++
CFLAGS				:= -std=c89 -Wall -Wextra -pedantic -fsanitize=address -g3 -O0
CXXFLAGS			:= -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -g3 -O0
LFLAGS				:= -fsanitize=address

SRC_DIR				:= ../src
LIB_DIR				:= ../dependencies
INC_DIR				:= -I../include -I$(LIB_DIR)/libft
OBJ_DIR				:= build
DEP_DIR				:= build

LIBFT_DIR			:= $(LIB_DIR)/libft
LIBFT_LIB			:= $(LIBFT_DIR)/libft.a

FILE_NAMES			:= parser.c memory.c assert.c pr_convert.c simple_tests.cpp parser_tests.cpp parser_tests_tokenizer.cpp

PARSER_FILE_NAMES	:= parser.c memory.c assert.c simple_tests.cpp parser_test.cpp

SOURCES				:= $(patsubst %.c,$(SRC_DIR)/%.c,$(FILE_NAMES))
OBJECTS				:= $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(patsubst %.c,$(OBJ_DIR)/%.o,$(FILE_NAMES)))

ifndef test
$(error "no test specified")
endif

ifeq ($(test), parser)
	FILE_NAMES		:= $(PARSER_FILE_NAMES)
else
$(error "no such test "$(test)"")
endif

all: $(TARGET)

$(TARGET): $(LIBFT_LIB) $(OBJECTS)
	@echo $(SOURCES)
	$(LINK_CMD) -o $(TARGET) $(OBJECTS) $(LFLAGS) -L$(LIBFT_DIR) -lft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $< -MMD $(INC_DIR)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD $(INC_DIR)

$(LIBFT_LIB):
	${MAKE} -C $(LIBFT_DIR)

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

re: clean all

-include $(DEPENDS)
.PHONY: all clean re