/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   die.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:34 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:34 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void
	sh_abort(void)
{
	kill(0, SIGTERM);
}

/* TODO: Ask Nicolas for fprintf permission */
void
	sh_assert_impl(int test)
{
	if (!test)
	{
		// TODO: use shell name from argv[0]
		// TODO: use stderr from t_minishell
		ft_fprintf(STDERR_FILENO, "Assertion failed!\n");
		exit(EXIT_FAILURE);
	}
}

void
	sh_check(int test, const char *s)
{
	if (!test)
	{
		// TODO: use shell name from argv[0]
		// TODO: use stderr from t_minishell
		perror(s);
		exit(EXIT_FAILURE);
	}
}
