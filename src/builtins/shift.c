/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   shift.c                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:16 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:16 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

#include <stdlib.h>

static void
	sh_shift_once(void)
{
	long	i;

	i = 1;
	free(sh()->args[i]);
	while (sh()->args[i] != NULL)
	{
		sh()->args[i] = sh()->args[i + 1];
		i += 1;
	}
}

int
	sh_shift(int argc, char **argv)
{
	long	shift_count;

	if (argc >= 3)
	{
		sh_err2("shift", "too many arguments");
		return (1);
	}
	if (sh_atol(argv[1], &shift_count) < 0)
	{
		sh_err2("shift", "numeric argument required");
		return (1);
	}
	if (shift_count < 0)
	{
		sh_err2("shift", "shift count out of range");
		return (1);
	}
	while (shift_count)
	{
		sh_shift_once();
		shift_count -= 1;
	}
	return (0);
}
