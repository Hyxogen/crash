/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   new_input.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/21 15:31:06 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 16:19:42 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "minishell.h"
# include <sys/types.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# elif (BUFFER_SIZE <= 0)
#  error BUFFER_SIZE must not be less than or equal to 0
# endif

typedef struct s_input	t_input;

typedef ssize_t (*t_read_line_proc)(t_input *in, char **lp);

typedef struct s_file_handle {
	int		fd;
	size_t	size;
	size_t	beg;
	size_t	end;
	char	*buf;
}	t_file_handle;

struct s_input {
	int					more;
	t_file_handle		*file_handle;
	const char			*string_handle;
	t_read_line_proc	proc;
};

ssize_t	_input_readline_proc(t_input *in, char **lp);
ssize_t	_input_file_proc(t_input *in, char **lp);
ssize_t _input_string_proc(t_input *in, char **lp);

void	input_setup_string(t_input *in, const char *str);
void	input_setup_file(t_input *in, int fd);
void	input_setup_readline(t_input *in);

ssize_t	input_get_line(t_input *in, char **lp);

#endif