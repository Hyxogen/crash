/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   main.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "lexer.h"
#include "input.h"
#include "commander.h"
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

void
	run(t_minishell *sh, t_input *in)
{
	t_source	src;
	t_lexer		lex;
	t_parser	pr;
	t_snode		*node;

	src_init(&src, in);
	lex_init(&lex);
	pr_init(&pr);
	lex.src = &src;
	pr.lexer = &lex;
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
			commandeer(sh, node, NULL);
		node_destroy(node);
		if (pr.lexer->error)
			printf("Syntax error\n");
	}
	pr_destroy(&pr);
}

int
	main(int argc, char **argv, char **envp)
{
	t_minishell	sh;
	t_input		in;
	char		*tmp;
	int			fd;

	(void) argc;
	tmp = getcwd(NULL, 0);
	sh.self = sh_join_path(tmp, argv[0]);
	sh.builtins = NULL;
	sh.builtins_size = 0;
	sh.args = argv + argc;
	sh.interactive = 1;
	free(tmp);
	sh_env_init(&sh, envp);
	cm_enable_reaper(&sh);
	if (argc >= 2)
	{
		fd = open(argv[1], O_RDONLY);
		sh.args = argv + 2;
		sh.interactive = 0;
		input_new(&in, in_file, (void*)(unsigned long long) fd);
	}
	else
		input_new(&in, in_readline, NULL);
	return (run(&sh, &in), EXIT_SUCCESS);
}
