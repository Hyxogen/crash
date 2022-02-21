/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   readline_proc.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 16:06:07 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 16:27:59 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_input.h"

#include "libft.h"
#include <readline/readline.h>

const char
	*input_get_prompt(const t_input *in)
{
}

ssize_t
	_input_readline_proc(t_input *in, char **lp)
{
	*lp = readline(input_get_prompt(in));
	if (!*lp)
		return (0);
	return (ft_strlen(*lp));
}
