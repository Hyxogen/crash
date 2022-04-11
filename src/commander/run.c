#include "minishell.h"
#include "commander.h"
#include "parser.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

t_snode
	*pr_parse(t_parser *pr)
{
	t_snode	*node;
	t_snode	*child;

	node = snode(sx_none);
	if (!pr_complete_cmd(pr, node))
	{
		node_destroy(node);
		return (NULL);
	}
	sh_assert(node->childs_size == 1);
	child = node->childs[0];
	node->childs_size = 0;
	node_destroy(node);
	return (child);
}

int
	sh_cm_run( t_input *in)
{
	t_source	src;
	t_lexer		lex;
	t_parser	pr;
	t_snode		*node;
	int			std_io[3];

	src_init(&src, in);
	lex_init(&lex);
	pr_init(&pr);
	lex.src = &src;
	pr.lexer = &lex;
	std_io[SH_STDIN_INDEX] = STDIN_FILENO;
	std_io[SH_STDOUT_INDEX] = STDOUT_FILENO;
	std_io[SH_STDERR_INDEX] = STDERR_FILENO;
	while (!pr.lexer->error)
	{
		pr.lexer->error = 0;
		if (pr.current.id != tk_invalid)
			token_destroy(&pr.current);
		pr_next_token(&pr);
		if (pr.current.id == tk_invalid || pr.current.id == tk_null)
			break ;
		node = pr_parse(&pr);
		if (node != NULL)
			commandeer(node, std_io);
		node_destroy(node);
		if (pr.lexer->error)
			ft_fprintf(sh()->io[SH_STDERR_INDEX], "%s: Syntax error\n", sh()->name);
	}
	pr_destroy(&pr);
	// TODO: errors?
	return (0);
}
