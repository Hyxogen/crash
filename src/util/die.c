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
		printf("Assertion failed!\n");
		exit(EXIT_FAILURE);
	}
}

void
	sh_check(int test, const char *s)
{
	if (!test)
	{
		perror(s);
		exit(EXIT_FAILURE);
	}
}
