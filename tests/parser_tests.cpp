#include "simple_tests.hpp"
#include <cstring>


extern "C" {
	#include "minishell.h"

	t_snode	*pr_bang(t_parser *pr);
	t_snode	*pr_newline(t_parser *pr);
	t_snode	*pr_pipe(t_parser *pr);
	t_snode	*pr_and_if(t_parser *pr);
	t_snode	*pr_or_if(t_parser *pr);
	t_snode	*pr_cmd_name(t_parser *pr);
	t_snode	*pr_cmd_word(t_parser *pr);
	t_snode	*pr_and(t_parser *pr);
	t_snode	*pr_semicolon(t_parser *pr);

	t_snode *pr_newline_list(t_parser *pr);
	t_snode	*pr_linebreak(t_parser *pr);
	t_snode *pr_separator_op(t_parser *pr);
	t_snode *pr_separator(t_parser *pr);
	t_snode	*pr_simple_command(t_parser *pr);
	t_snode	*pr_pipe_sequence(t_parser *pr);
	t_snode	*pr_pipeline(t_parser *pr);
	t_snode	*pr_and_or(t_parser *pr);

	int	pr_expect_node(t_parser *pr, t_snode *root, t_snode *(*func)(t_parser*), int forward);
	int	pr_next_token(t_parser *pr);

	void node_resize_childs(t_snode *node, size_t newsize);
	void node_destroy(t_snode *node);
	t_snode	*node_create_empty(void);
	t_snode	*node_create_id(t_syntax_id id);
	void node_add_child(t_snode *node, t_snode *child);
}

#pragma clang diagnostic ignored "-Wwritable-strings"

void add_token(t_token token);
void add_token(t_token_id id, char *str);
void clear_tokens();
size_t get_size();

void parser_setup(t_parser &parser) {
	std::memset(&parser, 0, sizeof(t_parser));
	parser.tokenizer = (t_tokenizer *) std::malloc(sizeof(t_tokenizer));
}

void parser_destroy(t_parser &parser) {
	std::free(parser.tokenizer);
}

SIMPLE_TEST(next_token) {
	t_parser parser;

	clear_tokens();
	parser_setup(parser);
	for (int i = word; i <= op_semi; i++)
		add_token((t_token_id) i, (char *) 0);
	
	for (int i = word; i <= op_semi; i++) {
		pr_next_token(&parser);
		ASSERT_TRUE(parser.current_ret != 0);
		ASSERT_TRUE(parser.current != 0);
	}
	for (int i = 0; i < 10; i++) {
		pr_next_token(&parser);
		ASSERT_EQUAL(0, parser.current_ret);
	}
}

SIMPLE_TEST(bang_symbol) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	parser_setup(parser);
	add_token(bang, "!");
	add_token(newline, "\n");
	pr_next_token(&parser);
	node = pr_bang(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_bang);
	pr_next_token(&parser);
	node = pr_bang(&parser);
	ASSERT_TRUE(node == NULL);
	parser_destroy(parser);
}

SIMPLE_TEST(newline_symbol) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	parser_setup(parser);
	add_token(newline, "\n");
	add_token(op_lessand, "<&");
	pr_next_token(&parser);
	node = pr_newline(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_newline);
	pr_next_token(&parser);
	node = pr_newline(&parser);
	ASSERT_TRUE(node == NULL);
	parser_destroy(parser);
}

SIMPLE_TEST(pipe_symbol) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	parser_setup(parser);
	add_token(op_pipe, "|");
	add_token(op_or_if, "||");
	pr_next_token(&parser);
	node = pr_pipe(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_pipe);
	pr_next_token(&parser);
	node = pr_pipe(&parser);
	ASSERT_TRUE(node == NULL);
}

SIMPLE_TEST(or_if_symbol) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	parser_setup(parser);
	add_token(op_or_if, "||");
	add_token(op_rparen, "}");
	pr_next_token(&parser);
	node = pr_or_if(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_or_if);
	pr_next_token(&parser);
	node = pr_or_if(&parser);
	ASSERT_TRUE(node == NULL);
}

SIMPLE_TEST(and_if_symbol) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	parser_setup(parser);
	add_token(op_and_if, "&&");
	add_token(op_clobber, ">|");
	pr_next_token(&parser);
	node = pr_and_if(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_and_if);
	pr_next_token(&parser);
	node = pr_and_if(&parser);
	ASSERT_TRUE(node == NULL);
	parser_destroy(parser);
}

SIMPLE_TEST(node_create_empty) {
	t_snode *node;

	node = node_create_empty();
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(node->childs_size, (size_t) 0);
	node_destroy(node);
}

SIMPLE_TEST(node_resize_childs) {
	t_snode *node;

	node = node_create_empty();
	node_resize_childs(node, 10);
	ASSERT_EQUAL((size_t) 10, node->childs_capacity);
	node_resize_childs(node, 20);
	ASSERT_EQUAL((size_t) 20, node->childs_capacity);
	node_destroy(node);
}

SIMPLE_TEST(node_add_child) {
	t_snode *root;

	root = node_create_empty();
	node_add_child(root, node_create_id(sx_list));
	ASSERT_EQUAL((size_t) 1, root->childs_size);
	ASSERT_EQUAL(sx_list, root->childs[0]->type);
	ASSERT_EQUAL((size_t) 0, root->childs[0]->childs_capacity)

	node_add_child(root, node_create_id(sx_pipe_sequence));
	ASSERT_EQUAL((size_t) 2, root->childs_size);
	ASSERT_EQUAL(sx_list, root->childs[0]->type);
	ASSERT_EQUAL(sx_pipe_sequence, root->childs[1]->type);
	ASSERT_EQUAL((size_t) 0, root->childs[0]->childs_capacity)
	ASSERT_EQUAL((size_t) 0, root->childs[1]->childs_capacity)
	node_destroy(root);
}

SIMPLE_TEST(expect_node) {
	t_parser parser;
	t_snode *root;

	clear_tokens();
	parser_setup(parser);
	root = node_create_empty();

	add_token(bang, "!");
	add_token(op_and_if, "&&");
	pr_next_token(&parser);
	ASSERT_EQUAL((size_t) 0, root->childs_size);
	ASSERT_EQUAL(1, pr_expect_node(&parser, root, pr_bang, 0));
	ASSERT_EQUAL((size_t) 1, root->childs_size);
	ASSERT_EQUAL(1, pr_expect_node(&parser, root, pr_bang, 1));
	ASSERT_EQUAL((size_t) 2, root->childs_size);
	ASSERT_EQUAL(sx_bang, root->childs[0]->type);

	ASSERT_EQUAL(0, pr_expect_node(&parser, root, pr_bang, 0));
	ASSERT_EQUAL(0, pr_expect_node(&parser, root, pr_bang, 1));
	ASSERT_EQUAL((size_t) 2, root->childs_size);

	ASSERT_EQUAL(1, pr_expect_node(&parser, root, pr_and_if, 0));
	ASSERT_EQUAL((size_t) 3, root->childs_size);
	ASSERT_EQUAL(1, pr_expect_node(&parser, root, pr_and_if, 1));
	ASSERT_EQUAL((size_t) 4, root->childs_size);
	ASSERT_EQUAL(sx_bang, root->childs[0]->type);
	ASSERT_EQUAL(sx_and_if, root->childs[2]->type);

	clear_tokens();
	node_destroy(root);
	root = node_create_empty();
	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(op_pipe, "|");
	add_token(word, "echo");

	pr_next_token(&parser);

	ASSERT_EQUAL(1, pr_expect_node(&parser, root, pr_newline_list, 0));

	ASSERT_EQUAL((size_t) 1,  root->childs_size);
	ASSERT_EQUAL(sx_newline_list, root->childs[0]->type);

	ASSERT_EQUAL((size_t) 2, root->childs[0]->childs_size);
	ASSERT_EQUAL(sx_newline_list, root->childs[0]->type);
	ASSERT_EQUAL(sx_newline, root->childs[0]->childs[0]->type);

	ASSERT_EQUAL((size_t) 2, root->childs[0]->childs[1]->childs_size);
	ASSERT_EQUAL(sx_newline_list, root->childs[0]->childs[1]->type);
	ASSERT_EQUAL(sx_newline, root->childs[0]->childs[1]->childs[0]->type);

	ASSERT_EQUAL((size_t) 1, root->childs[0]->childs[1]->childs[1]->childs_size);
	ASSERT_EQUAL(sx_newline_list, root->childs[0]->childs[1]->childs[1]->type);
	ASSERT_EQUAL(sx_newline, root->childs[0]->childs[1]->childs[1]->childs[0]->type);

	ASSERT_EQUAL((int) op_pipe, parser.current->id);
	node_destroy(root);
	parser_destroy(parser);
}

SIMPLE_TEST(newline_list) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	parser_setup(parser);

	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(op_pipe, "|");
	add_token(word, "echo");

	pr_next_token(&parser);

	node = pr_newline_list(&parser);
	ASSERT_TRUE(node != NULL);

	ASSERT_EQUAL((size_t) 2,  node->childs_size);
	ASSERT_EQUAL(sx_newline_list, node->type);
	ASSERT_EQUAL(sx_newline, node->childs[0]->type);

	ASSERT_EQUAL((size_t) 2, node->childs[1]->childs_size);
	ASSERT_EQUAL(sx_newline_list, node->childs[1]->type);
	ASSERT_EQUAL(sx_newline, node->childs[1]->childs[0]->type);

	ASSERT_EQUAL((size_t) 1, node->childs[1]->childs[1]->childs_size);
	ASSERT_EQUAL(sx_newline_list, node->childs[1]->childs[1]->type);
	ASSERT_EQUAL(sx_newline, node->childs[1]->childs[1]->childs[0]->type);

	ASSERT_EQUAL((int) op_pipe, parser.current->id);
	node_destroy(node);
	parser_destroy(parser);
}

SIMPLE_TEST(linebreak) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	node = pr_linebreak(&parser);
	ASSERT_TRUE(node != NULL);


	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(op_pipe, "|");
	pr_next_token(&parser);


	node = pr_linebreak(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_linebreak, node->type);

	ASSERT_EQUAL((size_t) 1,  node->childs_size);
	ASSERT_EQUAL(sx_newline_list, node->childs[0]->type);

	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_newline_list, node->childs[0]->type);
	ASSERT_EQUAL(sx_newline, node->childs[0]->childs[0]->type);

	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs[1]->childs_size);
	ASSERT_EQUAL(sx_newline_list, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL(sx_newline, node->childs[0]->childs[1]->childs[0]->type);

	ASSERT_EQUAL((size_t) 1, node->childs[0]->childs[1]->childs[1]->childs_size);
	ASSERT_EQUAL(sx_newline_list, node->childs[0]->childs[1]->childs[1]->type);
	ASSERT_EQUAL(sx_newline, node->childs[0]->childs[1]->childs[1]->childs[0]->type);
	node_destroy(node);

	ASSERT_EQUAL((int) op_pipe, parser.current->id);
	node = pr_linebreak(&parser);
	ASSERT_TRUE(node == NULL);

	parser_destroy(parser);
}

SIMPLE_TEST(cmd_name) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	add_token(word, "echo");
	add_token(word, "ls");
	add_token(word, "hcf");
	add_token(kw_while, "while");

	pr_next_token(&parser);
	node = pr_cmd_name(&parser);
	ASSERT_EQUAL(sx_cmd_name, node->type);
	node_destroy(node);

	pr_next_token(&parser);
	node = pr_cmd_name(&parser);
	ASSERT_EQUAL(sx_cmd_name, node->type);
	node_destroy(node);
	
	pr_next_token(&parser);
	node = pr_cmd_name(&parser);
	ASSERT_EQUAL(sx_cmd_name, node->type);
	node_destroy(node);

	pr_next_token(&parser);
	node = pr_cmd_name(&parser);
	ASSERT_TRUE(node == NULL);

	parser_destroy(parser);
}

SIMPLE_TEST(cmd_word) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	add_token(word, "echo");
	add_token(word, "ls");
	add_token(word, "hcf");
	add_token(kw_while, "while");

	pr_next_token(&parser);
	node = pr_cmd_word(&parser);
	ASSERT_EQUAL(sx_cmd_word, node->type);
	node_destroy(node);

	pr_next_token(&parser);
	node = pr_cmd_word(&parser);
	ASSERT_EQUAL(sx_cmd_word, node->type);
	node_destroy(node);
	
	pr_next_token(&parser);
	node = pr_cmd_word(&parser);
	ASSERT_EQUAL(sx_cmd_word, node->type);
	node_destroy(node);

	pr_next_token(&parser);
	node = pr_cmd_word(&parser);
	ASSERT_TRUE(node == NULL);

	parser_destroy(parser);
}

SIMPLE_TEST(and) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	add_token(op_and, "&");
	add_token(word, "ls");
	add_token(op_and, "&");
	add_token(kw_while, "while");

	pr_next_token(&parser);
	node = pr_and(&parser);
	ASSERT_EQUAL(sx_and, node->type);
	node_destroy(node);

	pr_next_token(&parser);
	node = pr_and(&parser);
	ASSERT_TRUE(node == NULL);

	pr_next_token(&parser);
	node = pr_and(&parser);
	ASSERT_EQUAL(sx_and, node->type);
	node_destroy(node);
	
	pr_next_token(&parser);
	node = pr_and(&parser);
	ASSERT_TRUE(node == NULL);

	parser_destroy(parser);
}

SIMPLE_TEST(semicolon) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	add_token(op_semi, ";");
	add_token(word, "ls");
	add_token(op_semi, ";");
	add_token(kw_while, "while");

	pr_next_token(&parser);
	node = pr_semicolon(&parser);
	ASSERT_EQUAL(sx_semicolon, node->type);
	node_destroy(node);

	pr_next_token(&parser);
	node = pr_semicolon(&parser);
	ASSERT_TRUE(node == NULL);

	pr_next_token(&parser);
	node = pr_semicolon(&parser);
	ASSERT_EQUAL(sx_semicolon, node->type);
	node_destroy(node);
	
	pr_next_token(&parser);
	node = pr_semicolon(&parser);
	ASSERT_TRUE(node == NULL);

	parser_destroy(parser);
}

SIMPLE_TEST(separator_op) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	node = pr_separator_op(&parser);
	ASSERT_TRUE(node == NULL);

	add_token(op_semi, ";");
	add_token(word, "ls");
	add_token(op_and, "&");
	add_token(kw_while, "while");

	pr_next_token(&parser);
	node = pr_separator_op(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_separator_op, node->type);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_semicolon, node->childs[0]->type);
	node_destroy(node);

	node = pr_separator_op(&parser);
	ASSERT_TRUE(node == NULL);

	pr_next_token(&parser);
	node = pr_separator_op(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_separator_op, node->type);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_and, node->childs[0]->type);
	node_destroy(node);

	node = pr_separator_op(&parser);
	ASSERT_TRUE(node == NULL);

	parser_destroy(parser);
}

SIMPLE_TEST(separator) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	node = pr_separator(&parser);
	ASSERT_TRUE(node == NULL);

	add_token(op_semi, ";");
	pr_next_token(&parser);
	node = pr_separator(&parser);

	clear_tokens();
	add_token(op_semi, ";");
	add_token(newline, "\n"); 
	pr_next_token(&parser);
	node = pr_separator(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_separator, node->type);
	ASSERT_EQUAL((size_t) 2, node->childs_size);
	ASSERT_EQUAL(sx_separator_op, node->childs[0]->type);
	ASSERT_EQUAL(sx_linebreak, node->childs[1]->type);
	node_destroy(node);

	clear_tokens();
	add_token(op_and, "&");
	add_token(newline, "\n");
	pr_next_token(&parser);
	node = pr_separator(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_separator, node->type);
	ASSERT_EQUAL((size_t) 2, node->childs_size);
	ASSERT_EQUAL(sx_separator_op, node->childs[0]->type);
	ASSERT_EQUAL(sx_linebreak, node->childs[1]->type);

	clear_tokens();
	add_token(op_clobber, ">|");
	add_token(newline, "\n");
	pr_next_token(&parser);
	node = pr_separator(&parser);
	ASSERT_TRUE(node == NULL);
	ASSERT_EQUAL((int) op_clobber, parser.current->id);
}

SIMPLE_TEST(simple_command) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	node = pr_simple_command(&parser);
	ASSERT_TRUE(node == NULL);

	clear_tokens();
	add_token(bang, "!");
	pr_next_token(&parser);
	node = pr_simple_command(&parser);
	ASSERT_TRUE(node == NULL);

	clear_tokens();
	add_token(word, "echo");
	pr_next_token(&parser);
	node = pr_simple_command(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_simple_cmd, node->type);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_cmd_name, node->childs[0]->type);
	node_destroy(node);
	/*TODO add tests for cmd_suffix and cmd_prefix*/
}

SIMPLE_TEST(pipe_sequence) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	parser_setup(parser);

	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node == NULL);

	clear_tokens();
	add_token(bang, "!");
	pr_next_token(&parser);
	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node == NULL);

	clear_tokens();
	add_token(word, "echo");
	pr_next_token(&parser);
	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_pipe_sequence, node->type);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_simple_cmd, node->childs[0]->type); /*TODO change this to compound command test*/
	node_destroy(node);

	clear_tokens();
	add_token(word, "ls");
	add_token(op_pipe, "|");
	add_token(word, "pbcopy");
	pr_next_token(&parser);
	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_pipe_sequence, node->type);
	ASSERT_EQUAL((size_t) 3, node->childs_size);
	ASSERT_EQUAL(sx_simple_cmd, node->childs[0]->type); /*TODO change this to compound command test*/
	node_destroy(node);

	clear_tokens();
	add_token(word, "ls");
	add_token(op_and, "&&");
	add_token(word, "echo");
	add_token(word, "Hello");
	add_token(word, "World!");
	pr_next_token(&parser);
	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_pipe_sequence, node->type);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_simple_cmd, node->childs[0]->type); /*TODO change this to compound command test*/
	node_destroy(node);

	clear_tokens();
	add_token(word, "ls");
	add_token(op_pipe, "|");
	add_token(kw_done, "done");
	pr_next_token(&parser);
	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node == NULL);

	clear_tokens();
	add_token(io_number, "1");
	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(newline, "\n");
	add_token(op_pipe, "|");
	add_token(word, "pbcopy");
	pr_next_token(&parser);
	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node == NULL);

	clear_tokens();
	add_token(word, "ls");
	add_token(op_clobber, ">|");
	add_token(newline, "\n");
	add_token(op_pipe, "|");
	add_token(word, "pbcopy");
	pr_next_token(&parser);
	node = pr_pipe_sequence(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_pipe_sequence, node->type);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_simple_cmd, node->childs[0]->type); /*TODO change this to compound command test*/
	node_destroy(node);
}

SIMPLE_TEST(pipeline) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	parser_setup(parser);

	node = pr_pipeline(&parser);
	ASSERT_TRUE(node == NULL);

	add_token(op_dless, "<");
	pr_next_token(&parser);
	node = pr_pipeline(&parser);
	ASSERT_TRUE(node == NULL);
	ASSERT_EQUAL((int) op_dless, parser.current->id);

	clear_tokens();
	add_token(word, "ls");
	add_token(op_pipe, "|");
	add_token(word, "pbcopy");
	pr_next_token(&parser);
	node = pr_pipeline(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_pipeline, node->type);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_pipe_sequence, node->childs[0]->type);
	node_destroy(node);

	clear_tokens();
	add_token(bang, "!");
	add_token(word, "ls");
	add_token(op_pipe, "|");
	add_token(word, "pbcopy");
	pr_next_token(&parser);
	node = pr_pipeline(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_pipeline, node->type);
	ASSERT_EQUAL((size_t) 2, node->childs_size);
	ASSERT_EQUAL(sx_bang, node->childs[0]->type);
	ASSERT_EQUAL(sx_pipe_sequence, node->childs[1]->type);
	node_destroy(node);
}

int main(int argc, char **argv) {
	SIMPLE_INIT(argc, argv);
	SIMPLE_RUN_ALL_TESTS();
	SIMPLE_CLEANUP();
	return (0);
}
