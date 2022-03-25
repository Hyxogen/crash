/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   input_string.c                                   -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:36 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:36 by csteenvo            :      ..            */
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
		return (-1);
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
