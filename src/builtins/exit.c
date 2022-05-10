/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   exit.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:12 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:12 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int
	sh_exit(int argc, char **argv)
{
	long	ex_code;

	ex_code = -1;
	if (argc <= 1)
		ex_code = 0;
	else if (argc >= 3)
	{
		sh_err2("exit", "too many arguments");
		ex_code = 0;
	}
	else if (sh_atol(argv[1], &ex_code) < 0)
	{
		sh_err3("exit", argv[1], "numeric argument required");
		ex_code = 0;
	}
	if (sh()->interactive)
		ft_putstr_fd("exit\n", sh()->io[SH_STDERR_INDEX]);
	exit(ex_code);
	return (-1);
}
