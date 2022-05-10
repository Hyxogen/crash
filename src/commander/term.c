/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   term.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:09 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:09 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <ft_printf.h>
#include <unistd.h>
#include <stdlib.h>

int
	commandeer_term(const t_snode *node, const int io[SH_STDIO_SIZE])
{
	pid_t	pid;

	if (node->flags & flag_and)
	{
		pid = sh_fork();
		if (pid == 0)
		{
			ft_fprintf(sh()->io[SH_STDOUT_INDEX],
				"%s: %d\n", sh()->name, getpid());
			exit(commandeer_inner(node, io));
		}
		sh()->last_bg_proc = pid;
		return (EXIT_SUCCESS);
	}
	return (commandeer_inner(node, io));
}
