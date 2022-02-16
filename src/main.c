/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 10:21:45 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/16 14:01:51 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
	main(int argc, char **argv)
{
	sh_assert(argc == 2);
	if (strcmp(argv[1], "pr") == 0)
		pr_debug();
	else if (strcmp(argv[1], "tk") == 0)
		lexer_debug();
	return (EXIT_SUCCESS);
}
