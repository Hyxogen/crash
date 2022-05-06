/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   commander.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:20:45 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:20:53 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include "minishell.h"
#include "memory.h"
#include <ft_printf.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

static t_commandeer_proc
	get_commandeer_proc(t_syntax_id id)
{
	static t_commandeer_proc	procs[46];

	procs[sx_list] = commandeer_inner;
	procs[sx_term] = commandeer_inner;
	procs[sx_command_list] = commandeer_inner;
	procs[sx_compound_list] = commandeer_inner;
	procs[sx_pipe_sequence] = execute_pipe_seq;
	procs[sx_term] = commandeer_term;
	procs[sx_and_if] = cm_and_if;
	procs[sx_or_if] = cm_or_if;
	if (procs[id] == NULL)
	{
		sh_backtrace(32);
		ft_fprintf(sh()->io[SH_STDERR_INDEX], "No command proc for %d\n", id);
		sh_assert(procs[id] != NULL);
	}
	return (procs[id]);
}

/* functions have the wrong return values */
int
	commandeer_inner(const t_snode *node, const int io[3])
{
	size_t	index;
	size_t	size;
	int		ret;

	ret = 0;
	size = node->childs_size;
	index = 0;
	while (index < size)
	{
		ret = get_commandeer_proc(node->childs[index]->type)
			(node->childs[index], io);
		index++;
	}
	return (!!ret);
}

/* TODO fork the entire term when it runs in the background */
int
	commandeer(const t_snode *node, const int io[3])
{
	int	ret_code;

	sh()->exec_count += 1;
	ret_code = get_commandeer_proc(node->type)(node, io);
	sh()->exec_count -= 1;
	return (ret_code);
}

pid_t
	cm_convert_retcode(int rc)
{
	return (-(rc + 1));
}
