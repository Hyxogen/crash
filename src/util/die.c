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

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void
	sh_abort(void)
{
	kill(0, SIGTERM);
}

void
	sh_assert_impl(int test)
{
	if (!test)
	{
		sh_err1("assertion failed");
		exit(EXIT_FAILURE);
	}
}

void
	sh_check(int test, const char *s)
{
	if (!test)
	{
		sh_err2(s, strerror(errno));
		exit(EXIT_FAILURE);
	}
}
