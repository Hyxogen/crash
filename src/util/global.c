/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   global.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:39 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:39 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* this is not a global */
t_minishell
	*sh(void)
{
	static t_minishell	shell;

	return (&shell);
}
