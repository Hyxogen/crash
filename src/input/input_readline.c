/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   input_readline.c                                 -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:34 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:34 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "minishell.h"
#include "commander.h"

#include "libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

int
	_input_readline_lex(char *str, t_token *token)
{
	t_input		in;
	t_source	src;
	t_lexer		lex;

	input_new(&in, in_string, str);
	src_init(&src, &in);
	lex_init(&lex);
	lex.src = &src;
	lex.src->force_newline = 0;
	lex.env_lexer = 1;
	lex.tok = token;
	token_init(lex.tok);
	lex.tok->id = tk_word;
	if (lex.src->cur == -1)
		lex_advance(&lex);
	lex_main(&lex);
	input_destroy(&in);
	return (lex.error || lex.src->cur != -1);
}

ssize_t
	_input_readline_line_proc(t_input *in, char **lp)
{
	t_token		token;
	char		*prompt;

	if (sh()->restart)
		return (0);
	if (in->more)
		prompt = sh_getenv("PS2", "");
	else
		prompt = sh_getenv("PS1", "");
	if (_input_readline_lex(prompt, &token))
	{
		*lp = NULL;
		return (-1);
	}
	child_reaper_lock();
	prompt = cm_expand_str(&token, NULL, ' ', 0);
	child_reaper_unlock();
	if (prompt == NULL)
		prompt = ft_strdup("$ ");
	*lp = readline(prompt);
	free(prompt);
	if (!*lp)
	{
		*lp = NULL;
		return (-1);
	}
	in->more = 1;
	history_append(*lp);
	return (ft_strlen(*lp));
}
