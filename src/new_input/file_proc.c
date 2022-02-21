/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   file_proc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:50:21 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 16:19:51 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_input.h"

#include "memory.h"

t_file_handle
	*start_read(int fd)
{
	t_file_handle	*ret;

	ret = sh_safe_malloc(sizeof(t_file_handle));
	ret->fd = fd;
	ret->size = BUFFER_SIZE;
	ret->buf = sh_safe_malloc(sizeof(char) * BUFFER_SIZE);
	ret->beg = 0;
	ret->end = 0;
	return (ret);
}

void
	close_read_handle(t_file_handle *rh)
{
	if (rh)
		free(rh->buf);
	free(rh);
}

static int
	push_strn(t_file_handle *rh, const char *str, size_t n)
{
	void	*tmp;

	if ((rh->beg + n) >= rh->size)
	{
		tmp = sh_safe_realloc(rh->buf, rh->size, rh->size + BUFFER_SIZE);
		rh->size += BUFFER_SIZE;
		rh->buf = tmp;
	}
	ft_memcpy(&rh->buf[rh->end], str, n);
	rh->end += n;
	return (1);
}

ssize_t
	_read_line(t_file_handle *fh, char **lp)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	read_size;
	char	*tmp;

	while (1)
	{
		tmp = ft_memchr(&fh->buf[fh->beg], '\n', fh->end - fh->beg);
		if (tmp)
		{
			read_size = tmp - &fh->buf[fh->beg];
			*lp = ft_strndup(&fh->buf[fh->beg], read_size);
			fh->beg += (tmp - &fh->buf[fh->beg]) + 1;
			return (read_size);
		}
		read_size = read(fh->fd, &buffer[0], BUFFER_SIZE);
		if (read_size < 0)
			return (-1);
		push_strn(fh, &buffer[0], read_size);
		if (read_size == 0)
		{
			tmp = ft_strndup(&fh->buf[fh->beg], fh->end - fh->beg);
			*lp = tmp;
			return (0);
		}
	}
}

ssize_t	_input_readline_proc(t_input *in, char **lp)
{
	return (_read_line(in->file_handle, lp));
}
