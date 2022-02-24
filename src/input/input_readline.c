/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_readline.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 16:06:07 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/24 15:23:48 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include "libft.h"
#include <readline/readline.h>

ssize_t
	_input_readline_line_proc(t_input *in, char **lp)
{
	if (in->more)
		*lp = readline(SH_INPUT_PROMPT_MORE);
	else
		*lp = readline(SH_INPUT_PROMPT_DEFAULT);
	if (!*lp)
	{
		*lp = NULL;
		return (-1);
	}
	in->more = 1;
	return (ft_strlen(*lp));
}
