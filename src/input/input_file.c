/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   input_file.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:32 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:32 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include "libft.h"
#include "memory.h"
#include <stdlib.h>
#include <unistd.h>

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
	close_file_handle(t_file_handle *rh)
{
	if (rh)
		free(rh->buf);
	free(rh);
}

static int
	_push_strn(t_file_handle *rh, const char *str, size_t n)
{
	void	*tmp;

	if ((rh->end + n) >= rh->size)
	{
		tmp = sh_safe_realloc(rh->buf, rh->size, rh->size * 2);
		rh->size = rh->size * 2;
		rh->buf = tmp;
		return (_push_strn(rh, str, n));
	}
	ft_memcpy(&rh->buf[rh->end], str, n);
	rh->end += n;
	return (1);
}

ssize_t
	_read_line(t_file_handle *fh, char **lp, ssize_t read_size)
{
	char	buffer[BUFFER_SIZE];
	char	*tmp;

	while (1)
	{
		tmp = ft_memchr(&fh->buf[fh->beg], '\n', fh->end - fh->beg);
		if (tmp)
		{
			read_size = tmp - &fh->buf[fh->beg];
			*lp = ft_strndup(&fh->buf[fh->beg], read_size);
			fh->beg += (tmp - &fh->buf[fh->beg]) + 1;
			return (ft_strlen(*lp));
		}
		read_size = read(fh->fd, &buffer[0], BUFFER_SIZE);
		if (read_size < 0 || (read_size == 0 && fh->beg == fh->end))
			return (-1);
		_push_strn(fh, &buffer[0], read_size);
		if (read_size == 0)
		{
			read_size = fh->end - fh->beg;
			*lp = ft_strndup(&fh->buf[fh->beg], read_size);
			fh->beg = fh->end;
			return (ft_strlen(*lp));
		}
	}
}

ssize_t	_input_file_line_proc(t_input *in, char **lp)
{
	return (_read_line(in->file_handle, lp, 0));
}
