/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   input.c                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:13 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:13 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>

void
	input_new(t_input *in, t_in_mode mode, void *param)
{
	in->more = 0;
	in->mode = mode;
	in->lst = sh_strlst_empty();
	if (mode == in_string)
	{
		in->string_handle = param;
		in->line_proc = _input_string_line_proc;
	}
	else if (mode == in_file)
	{
		in->file_handle = start_read((int)(unsigned long) param);
		in->line_proc = _input_file_line_proc;
	}
	else if (mode == in_readline)
	{
		in->line_proc = _input_readline_line_proc;
	}
}

void
	input_destroy(t_input *in)
{
	sh_strlst_clear(in->lst);
	if (in->mode == in_file)
		close_file_handle(in->file_handle);
}

ssize_t
	input_get_line(t_input *in, char **lp)
{
	size_t	i;
	ssize_t	ret;

	if (in->lst[0] == NULL)
	{
		ret = in->line_proc(in, lp);
		if (ret <= 0)
			return (ret);
		sh_strlst_clear(in->lst);
		in->lst = ft_split(*lp, '\n');
		free(*lp);
	}
	*lp = in->lst[0];
	i = 0;
	while (in->lst[i] != NULL)
	{
		in->lst[i] = in->lst[i + 1];
		i += 1;
	}
	return (ft_strlen(*lp));
}
