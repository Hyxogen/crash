#include "simple_tests.hpp"
#include <cstring>

extern "C" {
	#include "minishell.h"
	int	pr_expect_node(t_parser *pr, t_snode *root, t_snode *(*func)(t_parser*), int forward);
	int	pr_next_token(t_parser *pr);

	void node_resize_childs(t_snode *node, size_t newsize);
	void node_destroy(t_snode *node);
	void node_destroy_childs(t_snode *node);
	t_snode	*node_create(void);
	void node_add_child(t_snode *node, t_snode *child);
	t_snode	*node_init(t_snode *node, t_syntax_id syn_id);
	t_snode	*snode(t_syntax_id syn_id);

	int pr_token(t_parser *pr, t_snode *parent, t_syntax_id syn_id, t_token_id tk_id);
	int pr_io_file(t_parser *pr, t_snode *parent);
	int	pr_simple_cmd(t_parser *pr, t_snode *parent);
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

SIMPLE_TEST(node_init) {
	t_snode	node;

	node_init(&node, sx_word);
	ASSERT_EQUAL(sx_word, node.type);
	ASSERT_EQUAL((size_t) 0, node.childs_size);
	ASSERT_TRUE(node.token == NULL);
}


SIMPLE_TEST(node_create) {
	t_snode *node;

	node = node_create();
	node_init(node, sx_none);
	ASSERT_TRUE(node != NULL);
	node_destroy(node);
}

SIMPLE_TEST(node_resize_childs) {
	t_snode node;

	node_init(&node, sx_none);
	node_resize_childs(&node, 10);
	ASSERT_EQUAL((size_t) 10, node.childs_capacity);
	node_resize_childs(&node, 20);
	ASSERT_EQUAL((size_t) 20, node.childs_capacity);
	node_destroy_childs(&node);
}

SIMPLE_TEST(node_constructor) {
	t_snode *node;

	node = snode(sx_none);
	ASSERT_TRUE(node != NULL);
	ASSERT_EQUAL(sx_none, node->type);
	node_destroy(node);
}

SIMPLE_TEST(node_add_child) {
	t_snode node;

	node_init(&node, sx_none);
	node_add_child(&node, snode(sx_list));
	ASSERT_EQUAL((size_t) 1, node.childs_size);
	ASSERT_EQUAL(sx_list, node.childs[0]->type);
	ASSERT_EQUAL((size_t) 0, node.childs[0]->childs_capacity)

	node_add_child(&node, snode(sx_pipe_sequence));
	ASSERT_EQUAL((size_t) 2, node.childs_size);
	ASSERT_EQUAL(sx_list, node.childs[0]->type);
	ASSERT_EQUAL(sx_pipe_sequence, node.childs[1]->type);
	ASSERT_EQUAL((size_t) 0, node.childs[0]->childs_capacity)
	ASSERT_EQUAL((size_t) 0, node.childs[1]->childs_capacity)
	node_destroy_childs(&node);
}

SIMPLE_TEST(pr_token) {
	t_parser pr;
	t_snode	*node;

	parser_setup(pr);
	clear_tokens();

	pr_token(&pr, NULL, sx_none, token_id_null);

	add_token(word, "echo");
	add_token(op_pipe, "|");
	node = snode(sx_none);
	pr_next_token(&pr);
	pr_token(&pr, node, sx_newline, newline);
	ASSERT_EQUAL((size_t) 0, node->childs_size);

	pr_token(&pr, node, sx_word, word);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_word, node->childs[0]->type);

	pr_token(&pr, node, sx_pipe, op_pipe);
	ASSERT_EQUAL((size_t) 2, node->childs_size);
	ASSERT_EQUAL(sx_word, node->childs[0]->type);
	ASSERT_EQUAL(sx_pipe, node->childs[1]->type);
	node_destroy(node);
}

SIMPLE_TEST(pr_io_file) {
	t_parser pr;
	t_snode *node;

	parser_setup(pr);
	clear_tokens();

	node = snode(sx_none);

	pr_io_file(&pr, node);
	ASSERT_EQUAL((size_t) 0, node->childs_size);

	add_token(op_pipe, "|");
	add_token(word, "out.txt");
	pr_next_token(&pr);
	ASSERT_EQUAL(0, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 0, node->childs_size);
	ASSERT_EQUAL((int)op_pipe, pr.current->id);

	node = snode(sx_none);
	clear_tokens();
	add_token(op_less, "<");
	add_token(word, "/dev/null");
	add_token(op_and, "&&");
	pr_next_token(&pr);
	ASSERT_EQUAL(1, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_io_file, node->childs[0]->type);
	ASSERT_EQUAL(sx_less, node->childs[0]->childs[0]->type);
	ASSERT_EQUAL(sx_filename, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL((int) op_and, pr.current->id);
	node_destroy(node);

	node = snode(sx_none);
	clear_tokens();
	add_token(op_lessand, "<&");
	add_token(word, "/dev/null");
	add_token(op_and, "&&");
	pr_next_token(&pr);
	ASSERT_EQUAL(1, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_io_file, node->childs[0]->type);
	ASSERT_EQUAL(sx_lessand, node->childs[0]->childs[0]->type);
	ASSERT_EQUAL(sx_filename, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL((int) op_and, pr.current->id);
	node_destroy(node);

	node = snode(sx_none);
	clear_tokens();
	add_token(op_great, ">");
	add_token(word, "/dev/null");
	add_token(op_and, "&&");
	pr_next_token(&pr);
	ASSERT_EQUAL(1, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_io_file, node->childs[0]->type);
	ASSERT_EQUAL(sx_great, node->childs[0]->childs[0]->type);
	ASSERT_EQUAL(sx_filename, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL((int) op_and, pr.current->id);
	node_destroy(node);

	node = snode(sx_none);
	clear_tokens();
	add_token(op_greatand, ">&");
	add_token(word, "/dev/null");
	add_token(op_and, "&&");
	pr_next_token(&pr);
	ASSERT_EQUAL(1, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_io_file, node->childs[0]->type);
	ASSERT_EQUAL(sx_greatand, node->childs[0]->childs[0]->type);
	ASSERT_EQUAL(sx_filename, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL((int) op_and, pr.current->id);
	node_destroy(node);

	node = snode(sx_none);
	clear_tokens();
	add_token(op_dgreat, ">>");
	add_token(word, "/dev/null");
	add_token(op_and, "&&");
	pr_next_token(&pr);
	ASSERT_EQUAL(1, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_io_file, node->childs[0]->type);
	ASSERT_EQUAL(sx_dgreat, node->childs[0]->childs[0]->type);
	ASSERT_EQUAL(sx_filename, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL((int) op_and, pr.current->id);
	node_destroy(node);

	node = snode(sx_none);
	clear_tokens();
	add_token(op_lessgreat, "<>");
	add_token(word, "/dev/null");
	add_token(op_and, "&&");
	pr_next_token(&pr);
	ASSERT_EQUAL(1, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_io_file, node->childs[0]->type);
	ASSERT_EQUAL(sx_lessgreat, node->childs[0]->childs[0]->type);
	ASSERT_EQUAL(sx_filename, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL((int) op_and, pr.current->id);
	node_destroy(node);

	node = snode(sx_none);
	clear_tokens();
	add_token(op_clobber, ">|");
	add_token(word, "/dev/null");
	add_token(op_and, "&&");
	pr_next_token(&pr);
	ASSERT_EQUAL(1, pr_io_file(&pr, node));
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL((size_t) 2, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_io_file, node->childs[0]->type);
	ASSERT_EQUAL(sx_clobber, node->childs[0]->childs[0]->type);
	ASSERT_EQUAL(sx_filename, node->childs[0]->childs[1]->type);
	ASSERT_EQUAL((int) op_and, pr.current->id);
	node_destroy(node);
}

SIMPLE_TEST(pr_simple_cmd) {
	t_parser pr;
	t_snode *node;

	parser_setup(pr);
	clear_tokens();

	node = snode(sx_none);
	pr_simple_cmd(&pr, node);
	ASSERT_EQUAL((size_t) 0, node->childs_size);

	add_token(word, "ls");
	pr_next_token(&pr);
	pr_simple_cmd(&pr, node);
	ASSERT_EQUAL((size_t) 1, node->childs_size);
	ASSERT_EQUAL(sx_simple_cmd, node->childs[0]->type);
	ASSERT_EQUAL((size_t) 1, node->childs[0]->childs_size);
	ASSERT_EQUAL(sx_cmd_word, node->childs[0]->childs[0]->type);
}

int main(int argc, char **argv) {
	SIMPLE_INIT(argc, argv);
	SIMPLE_RUN_ALL_TESTS();
	SIMPLE_CLEANUP();
	return (0);
}
