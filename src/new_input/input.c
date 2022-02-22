/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:40:28 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 11:43:42 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_input.h"

void
	input_new(t_input *in, t_in_mode mode, void *param)
{
	in->more = 0;
	in->mode = mode;
	if (mode == in_string)
	{
		in->string_handle = param;
		in->line_proc = _input_file_line_proc;
	}
	else if (mode == in_file)
	{
		in->file_handle = start_read((int) param);
		in->line_proc = _input_file_line_proc;
	}
}

void
	input_destroy(t_input *in)
{
	if (in->mode == in_file)
		close_file_handle(in->file_handle);
}