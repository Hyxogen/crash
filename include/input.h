/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   input.h                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:56 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:56 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <sys/types.h>

# define SH_INPUT_PROMPT_MORE "> "
# define SH_INPUT_PROMPT_DEFAULT "$ "

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# elif (BUFFER_SIZE <= 0)
#  error BUFFER_SIZE must not be less than or equal to 0
# endif

typedef struct s_input	t_input;
typedef ssize_t			(*t_read_line_proc)(t_input *, char **);

typedef enum e_in_mode {
	in_string,
	in_file,
	in_readline
}	t_in_mode;

typedef struct s_file_handle {
	int		fd;
	size_t	size;
	size_t	beg;
	size_t	end;
	char	*buf;
}	t_file_handle;

struct s_input {
	int					more;
	const char			*string_handle;
	t_file_handle		*file_handle;
	t_read_line_proc	line_proc;
	t_in_mode			mode;
};

t_file_handle	*start_read(int fd);
void			close_file_handle(t_file_handle *rh);

ssize_t			_input_readline_line_proc(t_input *in, char **lp);
ssize_t			_input_file_line_proc(t_input *in, char **lp);
ssize_t			_input_string_line_proc(t_input *in, char **lp);

/* param is one of these: 
	*) Pointer to a c string
	*) A fd to a open file
	*) Nothing for readline */
void			input_new(t_input *in, t_in_mode mode, void *param);

void			input_destroy(t_input *in);

ssize_t			input_get_line(t_input *in, char **lp);

#endif
