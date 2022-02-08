/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   assert.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/08 09:20:23 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/08 15:55:41 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void
	sh_assert1(int test)
{
	if (!test)
	{
		exit(EXIT_FAILURE);
	}
}

void
	sh_assert2(int test, const char *str, const char *file, int line)
{
	if (!test)
	{
		fprintf(stderr, "assertion failed %s:%d: %s\n", file, line, str);
		raise(SIGTRAP);
	}
}
