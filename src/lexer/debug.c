#include "lexer.h"
#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <stdio.h>

static const char	*g_tk_names[] = {
	"tk_null",
	"tk_word",
	"tk_assword",
	"tk_name",
	"tk_newline",
	"tk_ionumber",
	"op_andif",
	"op_orif",
	"op_dsemi",
	"op_dless",
	"op_dgreat",
	"op_lessand",
	"op_greatand",
	"op_lessgreat",
	"op_dlessdash",
	"op_clobber",
	"kw_if",
	"kw_then",
	"kw_else",
	"kw_elif",
	"kw_fi",
	"kw_do",
	"kw_done",
	"kw_case",
	"kw_esac",
	"kw_while",
	"kw_until",
	"kw_for",
	"kw_lbrace",
	"kw_rbrace",
	"kw_bang",
	"kw_in",
	"op_and",
	"op_pipe",
	"op_lparen",
	"op_rparen",
	"op_less",
	"op_great",
	"op_semi"
};

static const char	*g_px_names[] = {
	"xp_word",
	"xp_parameter",
	"xp_command",
	"xp_arithmetic"
};

static void
	write_token(t_token *tok)
{
	ft_putstr_fd((char *) g_tk_names[tok->id], STDOUT_FILENO);
	ft_putstr_fd((char *) ": ", STDOUT_FILENO);
	write(STDOUT_FILENO, tok->string, tok->length);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void
	write_expansion(t_expansion *xp)
{
	ft_putchar_fd('\t', STDOUT_FILENO);
	if (xp->quoted)
		ft_putstr_fd("(quoted) ", STDOUT_FILENO);
	ft_putstr_fd((char *) g_px_names[xp->id], STDOUT_FILENO);
	ft_putstr_fd((char *) ": ", STDOUT_FILENO);
	write(STDOUT_FILENO, xp->string, xp->length);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void
	lexer_debug(void)
{
	t_input	in;
	t_lexer	lex;
	t_token	token;
	size_t	i;

	in.line = NULL;
	in.index = 0;
	in.more = 0;
	lexer_new(&lex, &in);
	while (1)
	{
		if (lexer_lex(&lex, &token) != 1)
			break ;
		write_token(&token);
		i = 0;
		while (i < token.count)
		{
			write_expansion(&token.xps[i]);
			i += 1;
		}
		token_delete(&token);
	}
	lexer_delete(&lex);
	free(in.line);
}
