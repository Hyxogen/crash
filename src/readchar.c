/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   readchar.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 12:11:24 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/08 12:42:51 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	
	sh_readchar(t_input *in)
{
	int	c;

	if (in->line == NULL && in->more)
		in->line = sh_readline(&in->rl, "> ");
	else if (in->line == NULL)
		in->line = sh_readline(&in->rl, "$ ");
	if (in->line == NULL)
		return (-1);
	c = (unsigned char) in->line[in->index];
	in->index += 1;
	in->more = 1;
	if (c == '\0')
	{
		free(in->line);
		in->line = NULL;
		in->index = 0;
		return ('\n');
	}
	return (c);
}
