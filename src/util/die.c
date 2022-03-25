/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   die.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 12:44:15 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/25 14:25:49 by dmeijer       ########   odam.nl         */
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
