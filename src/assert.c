/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   assert.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/08 09:20:23 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/08 09:24:05 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#ifdef SH_DEBUG
# include <stdio.h>
# include <signal.h>
# define sh_assert(condition) if (condition) { } else { printf("assertion failed %s:%d\n", __FILE__, __LINE__); raise(SIGTRAP); }
#else
void
	sh_assert(int condition) {
	if (!condition)
		exit(EXIT_FAILURE);
}
#endif
