/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   input_readline.c                                 -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:34 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:34 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

#include "libft.h"
#include <readline/readline.h>

/* use correct prompt */
ssize_t
	_input_readline_line_proc(t_input *in, char **lp)
{
	if (in->more)
		*lp = readline(SH_INPUT_PROMPT_MORE);
	else
		*lp = readline(SH_INPUT_PROMPT_DEFAULT);
	if (!*lp)
	{
		*lp = NULL;
		return (-1);
	}
	in->more = 1;
	return (ft_strlen(*lp));
}
