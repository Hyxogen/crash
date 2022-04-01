/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   input.c                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:31 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:31 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

void
	input_new(t_input *in, t_in_mode mode, void *param)
{
	in->more = 0;
	in->mode = mode;
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
	if (in->mode == in_file)
		close_file_handle(in->file_handle);
}

ssize_t
	input_get_line(t_input *in, char **lp)
{
	return (in->line_proc(in, lp));
}
