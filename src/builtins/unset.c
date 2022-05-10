/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   unset.c                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:18 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:18 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

int
	sh_unset_builtin(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		sh_unset(argv[i]);
		i += 1;
	}
	return (0);
}
