/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   readline.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/08 13:52:39 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/08 13:52:39 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

char
	*sh_readline(t_readline *rl, const char *prompt)
{
	(void) rl;
	return (readline(prompt));
}
