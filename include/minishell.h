/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/04 10:50:42 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/28 10:13:27 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
# include <sys/types.h>
# include <signal.h>

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

# define SH_ENV_EXPORT 1
# define SH_ENV_READONLY 2

# ifdef SH_DEBUG
#  include <assert.h>
#  define sh_assert assert
# else
#  define sh_assert sh_assert_impl
# endif

typedef struct s_minishell	t_minishell;
typedef struct s_envvar		t_envvar;
typedef struct s_builtin	t_builtin;
/* return code should be: -return_code - 1 */
typedef int					(*t_builtin_proc)(t_minishell *sh, int argc, char **argv, const int io[3]);

struct s_envvar
{
	char	*key;
	char	*value;
	char	*tmp_value;
	int		attr;
};

struct s_builtin
{
	const char		*key;
	t_builtin_proc	fn;
};

/* TODO: locals */
struct s_minishell
{
	t_envvar			*vars;
	size_t				vars_size;
	t_builtin			*builtins;
	size_t				builtins_size;
	char				*self;
	char				**args;
	int					interactive;
	struct sigaction	child_reaper;
};

char		*sh_join2(char *lhs, char delim, char *rhs);
void		sh_split2(char *str, char delim, char **lhs, char **rhs);
char		*sh_join_path(char *lhs, char *rhs);

t_envvar	*sh_setenv_int(t_minishell *sh, const char *key);
t_envvar	*sh_getenv_int(t_minishell *sh, const char *key, int create);
char		*sh_getenv(t_minishell *sh, const char *key, const char *def);
t_envvar	*sh_setenv(t_minishell *sh, const char *key, const char *value, int tmp);
char		**sh_env(t_minishell *sh);
void		sh_env_clean(t_minishell *sh);
void		sh_env_init(t_minishell *sh, char **env);

void		pr_debug(void);
void		sh_assert_impl(int test);
void		sh_check(int test, const char *s);
void		sh_abort(void);
void		sh_nop(void *ptr);

pid_t		sh_fork(void);
int			sh_execve(const char *path, char *const argv[], char *const envp[]);
int			sh_waitpid(pid_t pid, int *stat_loc, int options);
int			sh_pipe(int fildes[2]);
int			sh_dup2(int fildes, int fildes2);
int			sh_dup(int fildes);
int			sh_close(int fildes);
int			sh_open(const char *path, int oflag, mode_t mode);
int			sh_sigaction(int sig, const struct sigaction *act, struct sigaction *oact);

int			sh_exists(const char *filen);

int			sh_echo(t_minishell *sh, int argc, char **argv, const int io[3]);
int			sh_exit(t_minishell *sh, int argc, char **argv, const int io[3]);
int			sh_dot(t_minishell *sh, int argc, char **argv, const int io[3]);
int			sh_colon(t_minishell *sh, int argc, char **argv, const int io[3]);

#endif
