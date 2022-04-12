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

# include "parser.h"

# include <stddef.h>
# include <sys/types.h>
# include <signal.h>

/* Constant Rage Again SHell */
/* Can't Rest Again SHell */
/* Crying Rivers Again SHell*/
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
 * $USER is not in the sudoers file.  This incident will be reported.
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
 * $USER is in the sudoers file.  This incident will not be reported.
 */

// MAYBE TODO:
// break in pipe sequence

// FOR SURE TODO
// fork in multi-command pipe sequence
// 2.9.1; Command Search and Execution; 1.c (super extra special builtins)

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
typedef struct s_function	t_function;
/* return code should be: -return_code - 1 */
typedef int					(*t_builtin_proc)(int argc, char **argv);

struct s_envvar
{
	char	*key;
	char	*value;
	char	*tmp_value;
	int		attr;
};

struct s_builtin
{
	char			*key;
	t_builtin_proc	fn;
};

struct s_function
{
	char	*key;
	t_snode	*body;
};

struct s_minishell
{
	t_envvar			*vars;
	size_t				vars_size;
	const t_builtin		*builtins;
	size_t				builtins_size;
	t_function			*functions;
	size_t				functions_size;
	const t_builtin		*utilities;
	size_t				utilities_size;
	char				*self;
	char				**args;
	int					interactive;
	int					io[3];
	int					breaking;
	int					continuing;
	int					loop_depth;
	struct sigaction	child_reaper;
	char				*name;
	int					return_code;
};

char		*sh_join2(char *lhs, char delim, char *rhs);
void		sh_split2(char *str, char delim, char **lhs, char **rhs);
char		*sh_join_path(char *lhs, char *rhs);

t_envvar	*sh_setenv_int(const char *key);
t_envvar	*sh_getenv_int(const char *key, int create);
char		*sh_getenv(const char *key, const char *def);
t_envvar	*sh_setenv(const char *key, const char *value, int tmp);
char		**sh_env(void);
void		sh_env_clean(void);
void		sh_env_init(char **env);
void		sh_env_destroy(void);

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
ssize_t		sh_write(int fildes, const void *buf, size_t nbyte);

int			sh_exists(const char *filen);
void		sh_strlst_clear(char **strs);
char		*sh_strlst_join(char **strs, int delim);
char		**sh_strlst_new(char *str);
char		**sh_strlst_empty(void);
char		**sh_strlst_dup(char **strs);

int			sh_echo(int argc, char **argv);
int			sh_exit(int argc, char **argv);
int			sh_dot(int argc, char **argv);
int			sh_colon(int argc, char **argv);
int			sh_set(int argc, char **argv);
int			sh_break(int argc, char **argv);
int			sh_continue(int argc, char **argv);
int			sh_export(int argc, char **argv);
int			sh_shift(int argc, char **argv);

void		sh_backtrace(int count);
int			sh_atol(const char *str, long *v);

t_minishell	*sh(void);
void		sh_err1(const char *s1);
void		sh_err2(const char *s1, const char *s2);
void		sh_err3(const char *s1, const char *s2, const char *s3);

void		sh_init(char **argv, char **env);
void		sh_destroy(void);
void		sh_add_function(const char *key, t_snode *body);

#endif
