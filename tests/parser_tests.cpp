#include "simple_tests.hpp"
#include <cstring>

extern "C" {
	#include "minishell.h"

	t_snode	*pr_bang(t_parser *pr);
	t_snode	*pr_newline(t_parser *pr);
	t_snode	*pr_pipe(t_parser *pr);
	t_snode	*pr_and_if(t_parser *pr);
	t_snode	*pr_or_if(t_parser *pr);

	t_snode *pr_newline_list(t_parser *pr);
	t_snode	*pr_linebreak(t_parser *pr);
	
	int	pr_expect_node(t_parser *pr, t_snode *root, t_snode *(*func)(t_parser*));
	int	pr_next_token(t_parser *pr);

	void node_resize_childs(t_snode *node, size_t newsize);
	void node_destroy(t_snode *node);
	t_snode	*node_create_empty(void);
	t_snode	*node_create_id(t_syntax_id id);
	void node_add_child(t_snode *node, t_snode *child);
}

void add_token(t_token token);
void add_token(t_token_type type, t_token_id id, const char *str);
void clear_tokens();

void clear_parser(t_parser &parser) {
	std::memset(&parser, 0, sizeof(t_parser));
}

SIMPLE_TEST(next_token) {
	t_parser parser;

	clear_tokens();
	clear_parser(parser);
	for (int i = word; i <= op_semi; i++)
		add_token((t_token_type) 0, (t_token_id) i, (const char *) 0);
	
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
	clear_parser(parser);
	add_token(type_oper, bang, "!");
	add_token(type_word, newline, "\n");
	pr_next_token(&parser);
	node = pr_bang(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_bang);
	pr_next_token(&parser);
	node = pr_bang(&parser);
	ASSERT_TRUE(node == NULL);
}

SIMPLE_TEST(newline_symbol) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	clear_parser(parser);
	add_token(type_word, newline, "\n");
	add_token(type_oper, op_lessand, "<&");
	pr_next_token(&parser);
	node = pr_newline(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_newline);
	pr_next_token(&parser);
	node = pr_newline(&parser);
	ASSERT_TRUE(node == NULL);
}

SIMPLE_TEST(pipe_symbol) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	clear_parser(parser);
	add_token(type_oper, op_pipe, "|");
	add_token(type_oper, op_or_if, "||");
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
	clear_parser(parser);
	add_token(type_oper, op_or_if, "||");
	add_token(type_oper, op_rparen, "}");
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
	clear_parser(parser);
	add_token(type_oper, op_and_if, "&&");
	add_token(type_oper, op_clobber, ">|");
	pr_next_token(&parser);
	node = pr_and_if(&parser);
	ASSERT_TRUE(node != NULL);
	ASSERT_TRUE(node->type == sx_and_if);
	pr_next_token(&parser);
	node = pr_and_if(&parser);
	ASSERT_TRUE(node == NULL);
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
}

SIMPLE_TEST(expect_node) {
	t_parser parser;
	t_snode *root;

	clear_tokens();
	clear_parser(parser);
	root = node_create_empty();

	add_token(type_oper, bang, "!");
	add_token(type_oper, op_and_if, "&&");
	pr_next_token(&parser);
	ASSERT_EQUAL(1, pr_expect_node(&parser, root, pr_bang));
	ASSERT_EQUAL((size_t) 1, root->childs_size);
	ASSERT_EQUAL(sx_bang, root->childs[0]->type);

	ASSERT_EQUAL(0, pr_expect_node(&parser, root, pr_bang));
	ASSERT_EQUAL((size_t) 1, root->childs_size);

	ASSERT_EQUAL(1, pr_expect_node(&parser, root, pr_and_if));
	ASSERT_EQUAL((size_t) 2, root->childs_size);
	ASSERT_EQUAL(sx_bang, root->childs[0]->type);
	ASSERT_EQUAL(sx_and_if, root->childs[1]->type);
	node_destroy(root);
}

SIMPLE_TEST(newline_list) {
	t_parser parser;
	t_snode	*node;

	clear_tokens();
	clear_parser(parser);

	add_token(type_word, newline, "\n");
	add_token(type_word, newline, "\n");
	add_token(type_word, newline, "\n");

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
	node_destroy(node);
}

SIMPLE_TEST(linebreak) {
	t_parser parser;
	t_snode *node;

	clear_tokens();
	clear_parser(parser);

	node = pr_linebreak(&parser);
	ASSERT_TRUE(node != NULL);


	add_token(type_word, newline, "\n");
	add_token(type_word, newline, "\n");
	add_token(type_word, newline, "\n");
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
}

int main(int argc, char **argv) {
	SIMPLE_INIT(argc, argv);
	SIMPLE_RUN_ALL_TESTS();
	SIMPLE_CLEANUP();
	return (0);
}
