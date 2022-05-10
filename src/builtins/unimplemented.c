/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   unimplemented.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:17 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:17 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

int
	sh_unimplemented(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	sh_err2(argv[0], "not implemented");
	return (EXIT_FAILURE);
}
