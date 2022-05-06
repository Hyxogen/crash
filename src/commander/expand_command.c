#include "commander.h"

#include "minishell.h"
#include "memory.h"
#include "parser.h"
#include <unistd.h>
#include <stdlib.h>

size_t
	expand_command_sanitize(char *str, size_t size)
{
	size_t	i;
	size_t	j;

	while (size > 0 && str[size - 1] == '\n')
		size -= 1;
	i = 0;
	j = 0;
	while (i < size)
	{
		if (str[i] != '\0')
		{
			str[j] = str[i];
			j += 1;
		}
		else
			sh_err3("warning", "command substitution",
				"ignored null byte in input");
		i += 1;
	}
	return (j);
}

int
	expand_command_fd(t_expand *exp, pid_t pid, int fd)
{
	char	*str;
	size_t	size;
	int		ret;

	str = NULL;
	size = 0;
	ret = 1;
	while (ret > 0)
	{
		str = sh_safe_reallog(str, size, size + 4096);
		ret = read(fd, str + size, 4096);
		if (ret >= 0)
			size += ret;
	}
	size = expand_command_sanitize(str, size);
	str[size] = '\0';
	sh_close(fd);
	sh_waitpid(pid, NULL, 0);
	expansion_add_part(exp, sh_strlst_new(str), 0);
	return (0);
}

int
	expand_backtick_int(t_expand *exp, t_lexer *lex)
{
	t_parser	pr;
	t_snode		*node;
	int			result;

	pr_init(&pr);
	pr.lexer = lex;
	pr_next_token(&pr);
	node = snode(sx_none);
	pr_complete_cmdlst(&pr, node);
	sh_assert(node->childs_size == 1);
	result = expand_command(exp, node->childs[0]);
	node_destroy(node);
	pr_destroy(&pr);
	return (result);
}

int
	expand_command(t_expand *exp, t_snode *node)
{
	pid_t	pid;
	int		pipe_out[2];
	int		io[3];

	sh_pipe(pipe_out);
	pid = sh_fork();
	if (pid == 0)
	{
		sh_close(pipe_out[0]);
		sh_fdctl(pipe_out[1], SH_FD_FIOCLEX, 1);
		io[SH_STDIN_INDEX] = 0;
		io[SH_STDOUT_INDEX] = pipe_out[1];
		io[SH_STDERR_INDEX] = 2;
		commandeer(node, io);
		exit(EXIT_SUCCESS);
	}
	sh_close(pipe_out[1]);
	return (expand_command_fd(exp, pid, pipe_out[0]));
}

int
	expand_backtick(t_expand *exp, char *str)
{
	t_input		in;
	t_source	src;
	t_lexer		lex;
	int			result;

	input_new(&in, in_string, str);
	src_init(&src, &in);
	lex_init(&lex);
	lex.src = &src;
	result = expand_backtick_int(exp, &lex);
	input_destroy(&in);
	return (result);
}
