#include "minishell.h"
#include <unistd.h>
#include <libft.h>
#include <stdlib.h>

static const char	*g_pr_names[] = {
	"sx_none",
	"sx_and",
	"sx_cmd_name",
	"sx_cmd_word",
	"sx_semicolon",
	"sx_separator_op",
	"sx_separator",
	"sx_and_if",
	"sx_or_if",
	"sx_pipe",
	"sx_newline",
	"sx_newline_list",
	"sx_word",
	"sx_simple_cmd",
	"sx_pipe_sequence",
	"sx_pipeline",
	"sx_list",
	"sx_complete_cmd",
	"sx_and_or",
	"sx_linebreak",
	"sx_bang"
};

int		pr_complete_cmd(t_parser *pr);
int		pr_next_token(t_parser *pr);

static void
	pr_print(t_snode *node, size_t depth)
{
	size_t	i;

	i = 0;
	while (i < depth)
	{
		ft_putstr_fd((char*) "  ", STDOUT_FILENO);
		i += 1;
	}
	ft_putstr_fd((char*) g_pr_names[node->type], STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	i = 0;
	while (i < node->childs_size)
	{
		pr_print(node->childs[i], depth + 1);
		i += 1;
	}
}

t_snode
	*pr_parse(t_parser *pr)
{
	return pr_list(pr);
}

void
	pr_debug(void)
{
	t_input		in;
	t_tokenizer	tk;
	t_parser	pr;
	t_snode		*node;

	in.line = NULL;
	in.index = 0;
	in.more = 0;
	tk_create(&tk, &in);
	pr.tokenizer = &tk;
	pr.current = NULL;
	pr.current_ret = 0;
	pr.current_node = NULL;
	pr.syntax_tree = NULL;
	while (1)
	{
		pr_next_token(&pr);
		node = pr_parse(&pr);
		if (node == NULL)
			break ;
		pr_print(node, 0);
	}
	free(in.line);
}
