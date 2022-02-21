#include "minishell.h"
#include "parser.h"
#include <unistd.h>
#include <libft.h>
#include <stdlib.h>

static const char	*g_sx_names[] = {
	"sx_none",
	"sx_and",
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
	"sx_assword",
	"sx_simple_cmd",
	"sx_pipe_sequence",
	"sx_pipeline",
	"sx_list",
	"sx_complete_cmd",
	"sx_and_or",
	"sx_linebreak",
	"sx_bang",
	"sx_io_file",
	"sx_less",
	"sx_lessand",
	"sx_great",
	"sx_greatand",
	"sx_dgreat",
	"sx_lessgreat",
	"sx_clobber",
	"sx_filename",
	"sx_io_redirect",
	"sx_io_number",
	"sx_cmd_suffix",
	"sx_cmd_prefix",
	"sx_cmd",
	"sx_term",
	"sx_compound_list",
	"sx_subshell",
	"sx_brace_group",
	"sx_compound_cmd",
	"sx_while_clause",
	"sx_until_clause",
	"sx_do_group",
	"sx_if_clause",
	"sx_else_part",
	"sx_function_def",
	"sx_function_body",
	"sx_function_name",
	"sx_case_clause",
	"sx_case_list",
	"sx_case_item",
	"sx_pattern",
	"sx_for_clause",
	"sx_for_name",
	"sx_wordlist",
	"sx_condition",
	"sx_elif_part"
};

int		pr_complete_cmd(t_parser *pr, t_snode *parent);
int		pr_next_token(t_parser *pr);
t_snode	*snode(t_syntax_id syn_id);

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
	ft_putstr_fd((char*) g_sx_names[node->type], STDOUT_FILENO);

	if (!node->childs_size && node->token && node->token->string)
	{
		ft_putchar_fd(':', STDOUT_FILENO);
		ft_putstr_fd((char*) node->token->string, STDOUT_FILENO);
	}
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
	t_snode	*node;

	node = snode(sx_none);
	if (!pr_complete_cmd(pr, node))
		return (NULL);
	return (node->childs[0]);
}

void
	pr_debug(void)
{
	t_input		in;
	t_lexer		lx;
	t_parser	pr;
	t_snode		*node;

	in.line = NULL;
	in.index = 0;
	in.more = 0;
	lexer_new(&lx, &in);
	pr.lexer = &lx;
	pr.current = NULL;
	pr.next = NULL;
	pr.current_ret = 0;
	pr.next_ret = 0;
	pr.current_node = NULL;
	pr.syntax_tree = NULL;
	while (1)
	{
		pr_next_token(&pr);
		node = pr_parse(&pr);
		if (node != NULL)
			pr_print(node, 0);
		else
			break ;
	}
	free(in.line);
}
