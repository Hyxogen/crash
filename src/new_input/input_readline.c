/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_readline.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 16:06:07 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:36:29 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_input.h"

#include "libft.h"
#include <readline/readline.h>

ssize_t
	_input_readline_proc(t_input *in, char **lp)
{
	if (in->more)
		*lp = readline(SH_INPUT_PROMPT_MORE);
	else
		*lp = readline(SH_INPUT_PROMPT_DEFAULT);
	if (!*lp)
		return (0);
	return (ft_strlen(*lp));
}
