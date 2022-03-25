/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commander.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 14:47:35 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/25 10:47:44 by dmeijer       ########   odam.nl         */
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
	procs[sx_pipe_sequence] = commandeer_pipe_sequence;
	return (procs);
}

int
	commandeer_inner(t_minishell *sh, t_snode *node, void *data)
{
	size_t	index;
	size_t	size;
	int		ret;

	ret = 0;
	size = node->childs_size;
	index = 0;
	while (index < size)
	{
		ret = (ret || get_command_table()[node->childs[index]->type](sh, node->childs[index], data));
		index++;
	}
	return (!!ret);
}

int
	commandeer(t_minishell *sh, t_snode *node, void *data)
{
	return (get_command_table()[node->type](sh, node, data));
}