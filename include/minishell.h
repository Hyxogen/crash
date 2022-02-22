/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:50:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 14:37:59 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
/* Constant Rage Again SHell*/
/* https://pubs.opengroup.org/onlinepubs/009656399/toc.pdf */

# ifdef SH_DEBUG
#  define sh_assert(test) sh_assert2(test, #test, __FILE__, __LINE__)
# else
#  define sh_assert sh_assert1
# endif

typedef struct s_minishell	t_minishell;

struct s_minishell
{
	int	warning_silencer;
};

void	pr_debug(void);

void	sh_assert1(int test);
void	sh_assert2(int test, const char *str, const char *file, int line);

void	sh_nop(void *ptr);

#endif
