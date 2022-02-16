#ifndef INPUT_H
# define INPUT_H

# include <stddef.h>

typedef struct s_readline	t_readline;
typedef struct s_input		t_input;

struct s_readline
{
	int	warning_silencer;
};

struct s_input
{
	t_readline	rl;
	char		*line;
	size_t		index;
	int			more;
};

char	*input_readline(t_readline *rl, const char *prompt);
int		input_readchar(t_input *in);

#endif
