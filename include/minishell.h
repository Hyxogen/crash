/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:50:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/22 13:17:17 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Constant Rage Again SHell */
/* Can't Rest Again SHell */
/* https://pubs.opengroup.org/onlinepubs/009656399/toc.pdf */

/**
 * Errors:
 * Command entered, did no
 * Segmentation fault
 * 404 Command not found
 * Temporary failure in hostname resolution
 * Connection refused
 * Connection reset
 * 418 I'm a teapot
 * Not acceptable
 * Internal server error
 * Too early
 * Unavailable for legal reasons
 * Operation already in progress
 * Not a directory
 * Message too long
 * File is a directory
 * Function not implemented
 * The device power if off
 * Network is down
 * Bad message
 * Goodbye, World!
 * Process drowned
 * Process was squashed by a falling anvil
 * Process went of with a bang
 * Process was killed by magic
 * Snake? SNAAAAAAAAKE!!!
 * Illegal instruction
 * 
 * Success:
 * Command entered, did yes
 * Segmentation success
 * Bus ok
 * Kernel calm
 * Legal instruction
 * 200 Command found
 * Error: Command executed successfully
 * Error: Command executed with no errors
 * Temporary success in hostname resolution
 * Error: File Exists
 * Operation now in progress
 * Deadlock avoided
 * Function implemented
 * Good message
 * Hello, World!
 * A winner is you
 */

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
