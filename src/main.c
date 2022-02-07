/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 10:21:45 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/07 10:21:46 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

int
	main(void)
{
	t_input	in;

	in.line = NULL;
	in.index = 0;
	while (1)
	{
		int c = sh_readchar(&in, 1);
		if (c == 0)
			break;
		printf("%c", c);
	}
	return (EXIT_SUCCESS);
}
