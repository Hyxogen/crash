/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:50:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/21 11:32:03 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* https://pubs.opengroup.org/onlinepubs/009656399/toc.pdf */

# include <stddef.h>
# include "lexer.h"

# ifdef SH_DEBUG
#  define sh_assert(test) sh_assert2(test, #test, __FILE__, __LINE__)
# else
#  define sh_assert sh_assert1
# endif

typedef struct s_minishell	t_minishell;

struct s_minishell
{
	t_input		in;
};

char		*sh_readline(t_readline *rl, const char *prompt);
int			sh_readchar(t_input *in);

void		*sh_safe_malloc(size_t size);
void		*sh_safe_realloc(void *ptr, size_t old_size, size_t new_size);

void		pr_debug(void);

void		sh_assert1(int test);
void		sh_assert2(int test, const char *str, const char *file, int line);

#endif
