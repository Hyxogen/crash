/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_string.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:42:44 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/24 11:10:42 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include "libft.h"

ssize_t
	_input_string_line_proc(t_input *in, char **lp)
{
	const char	*str;
	const char	*newline;

	if (!in->string_handle)
		return (0);
	str = in->string_handle;
	newline = ft_strchr(str, '\n');
	if (!newline)
	{
		*lp = ft_strdup(in->string_handle);
		in->string_handle = NULL;
		return (ft_strlen(*lp));
	}
	*lp = ft_strndup(str, (newline - str) + 1);
	str = newline + 1;
	in->string_handle = str;
	return (ft_strlen(*lp));
}
