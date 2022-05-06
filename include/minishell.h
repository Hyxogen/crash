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
# include <limits.h>
# include <termios.h>

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
 * Substitution failure is an error
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
 * Substitution failure is not an error
 */

// MAYBE TODO:
// break in pipe sequence
// errors for unclosed stuff
// ! in PS1

/*
 * FOR SURE TODO:
 * handle all the shell variables
 * return value of lex_main is rarely checked
 * path name expansion (~, *)
 * disable SIGINT and SIGQUIT ignoring for internal commands
 * don't export invalid identifiers
 * shell flags (set -)
 * remove globals according to the subject/norminette
 */
# define SH_ENV_EXPORT 1
# define SH_ENV_READONLY 2

# ifdef SH_DEBUG
#  include <assert.h>
#  define sh_assert assert
# else
#  define sh_assert sh_assert_impl
# endif

typedef struct s_minishell		t_minishell;
typedef struct s_envvar			t_envvar;
typedef struct s_builtin		t_builtin;
typedef struct s_function		t_function;
typedef struct s_mega_termios	t_mega_termios;
typedef int						t_strinfo;
typedef struct s_string			t_string;
typedef struct s_stringlst		t_stringlst;
/* return code should be: -return_code - 1 */
typedef int						(*t_builtin_proc)(int argc, char **argv);

# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif

# ifndef SH_FD_FIOCLEX
#  define SH_FD_FIOCLEX 0x1
# endif

# ifndef SH_INVALID_INTERNAL_PID
#  define SH_INVALID_INTERNAL_PID 1
# endif
# ifndef SH_ERROR_INTERNAL_PID
#  define SH_ERROR_INTERNAL_PID 2
# endif

# ifndef SH_CLOSED_FD
#  define SH_CLOSED_FD -1
# endif

# ifndef SH_STDIO_SIZE
#  define SH_STDIO_SIZE 3
# endif
# ifndef SH_STDIN_INDEX
#  define SH_STDIN_INDEX 0
# elif SH_STDIN_INDEX >= SH_STDIO_SIZE
#  error "SH_STDIO_SIZE must be at least one larger than SH_STDIN_INDEX"
# endif
# ifndef SH_STDOUT_INDEX
#  define SH_STDOUT_INDEX 1
# elif SH_STDOUT_INDEX >= SH_STDIO_SIZE
#  error "SH_STDIO_SIZE must be at least one larger than SH_STDOUT_INDEX"
# endif
# ifndef SH_STDERR_INDEX
#  define SH_STDERR_INDEX 2
# elif SH_STDERR_INDEX >= SH_STDIO_SIZE
#  error "SH_STDIO_SIZE must be at least one larger than SH_STDERR_INDEX"
# endif

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

struct s_mega_termios {
	struct termios	term_attr[SH_STDIO_SIZE];
};

struct s_string {
	char		*str;
	t_strinfo	*info;
	size_t		size;
};

struct s_stringlst {
	t_string	*strings;
	size_t		size;
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
	char				*pwd;
	int					interactive;
	int					io[3];
	int					breaking;
	int					continuing;
	int					loop_depth;
	struct sigaction	child_reaper;
	char				*name;
	int					return_code;
	int					fd_flags[OPEN_MAX];
	char				*last_command;
	int					exec_count;
	int					restart;
};

char		*sh_join2(const char *lhs, char delim, const char *rhs);
void		sh_split2(char *str, char delim, char **lhs, char **rhs);
char		*sh_join_path(char *lhs, char *rhs);

t_envvar	*sh_setenv_int(const char *key);
t_envvar	*sh_getenv_int(const char *key, int create);
char		*sh_getenv(const char *key, const char *def);
t_envvar	*sh_setenv(const char *key, const char *value, int tmp);
void		sh_unset(const char *key);
char		**sh_env(void);
void		sh_env_clean(void);
void		sh_env_init(char **env);
void		sh_env_destroy(void);

void		pr_debug(void);
void		sh_assert_impl(int test);
void		sh_check(int test, const char *s);
void		sh_abort(void);
void		sh_nop(void *ptr);
void		sh_nop1(void);

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
int			sh_chdir(const char *path);
char		*sh_getcwd(void);

int			sh_exists(const char *filen);
void		sh_strlst_clear(char **strs);
char		*sh_strlst_join(char **strs, int delim);
char		**sh_strlst_new(char *str);
char		**sh_strlst_empty(void);
char		**sh_strlst_dup(char **strs);

void		sh_stringlst_begin(t_stringlst *lst);
void		sh_stringlst_add_string(t_stringlst *lst);
void		sh_stringlst_add_char(t_stringlst *lst, int ch, t_strinfo info);
void		sh_stringlst_end(t_stringlst *lst, char ***string, t_strinfo ***info);

int			sh_echo(int argc, char **argv);
int			sh_exit(int argc, char **argv);
int			sh_dot(int argc, char **argv);
int			sh_colon(int argc, char **argv);
int			sh_set(int argc, char **argv);
int			sh_break(int argc, char **argv);
int			sh_continue(int argc, char **argv);
int			sh_export(int argc, char **argv);
int			sh_shift(int argc, char **argv);
int			sh_getopts(int argc, char **argv);
int			sh_cd(int argc, char **argv);
int			sh_pwd(int argc, char **argv);
int			sh_unimplemented(int argc, char **argv);
int			sh_true(int argc, char **argv);
int			sh_false(int argc, char **argv);
int			sh_unset_builtin(int argc, char **argv);
int			sh_env_builtin(int argc, char **argv);

void		sh_backtrace(int count);
int			sh_atol(const char *str, long *v);
int			sh_arith_atol(const char *str, long *v);
void		sh_ltoa(long value, char *str, size_t len);

t_minishell	*sh(void);
void		sh_err1(const char *s1);
void		sh_err2(const char *s1, const char *s2);
void		sh_err3(const char *s1, const char *s2, const char *s3);

void		sh_init(char **argv, char **env);
void		sh_destroy(void);
void		sh_add_function(const char *key, t_snode *body);

void		sh_fdctl(int fd, int flag, int on);
void		sh_fd_before_exec(void);

void		history_append(const char *line);
const char	*history_get_last_command(void);
void		history_new_command(void);

void		setup_default_signal_handlers(void);
void		disable_kill_signals(void);

void		sh_get_term_attr(t_mega_termios *attr);
void		sh_set_term_attr(t_mega_termios *attr);
#endif
