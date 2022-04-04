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

#include "memory.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

static t_commandeer_proc
	*get_command_table(void)
{
	static t_commandeer_proc procs[46];

	procs[sx_list] = commandeer_inner;
	procs[sx_term] = commandeer_inner;
	procs[sx_command_list] = commandeer_inner;
	procs[sx_compound_list] = commandeer_inner;
	procs[sx_pipe_sequence] = commandeer_pipe_sequence;
	procs[sx_if_clause] = cm_if_clause;
	return (procs);
}

int
	commandeer_inner(t_minishell *sh, t_snode *node, const int io[3])
{
	size_t	index;
	size_t	size;
	int		ret;

	ret = 0;
	size = node->childs_size;
	index = 0;
	while (index < size)
	{
		ret = (ret || get_command_table()[node->childs[index]->type](sh, node->childs[index], io));
		index++;
	}
	return (!!ret);
}

/* TODO fork the entire term when it runs in the background */
int
	commandeer(t_minishell *sh, t_snode *node, const int io[3])
{
	return (get_command_table()[node->type](sh, node, io));
}

pid_t
	cm_convert_retcode(int rc)
{
	return (-(rc + 1));
}
