/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/07 10:21:45 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/02/08 15:46:20 by csteenvo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

const char *token_id_names[] = {
	"word",
	"assignment_word",
	"name",
	"newline",
	"io_number",
	"op_and_if",
	"op_or_if",
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
	"lbrace",
	"rbrace",
	"bang",
	"kw_in",
	"op_and",
	"op_pipe",
	"op_lparen",
	"op_rparen",
	"op_less",
	"op_great",
	"op_semi"
};

int
	main(void)
{
	t_input		in;
	t_tokenizer	tk;
	t_token		tok;

	in.line = NULL;
	in.index = 0;
	tk.in = &in;
	tk.tok = &tok;
	tk.ch = -1;
	tk.ch2 = -1;
	tk.bslash = 0;
	tk.quote = 0;
	while (1)
	{
		if (!tk_tokenize(&tk))
			break ;
		fprintf(stdout, "CH:%d CH2:%d BSLASH:%d QUOTE:%d\n",
			tk.ch, tk.ch2, tk.bslash, tk.quote);
		fprintf(stdout, "%s: ", token_id_names[tok.id]);
		fwrite(tok.str, 1, tok.len, stdout);
		fprintf(stdout, "\n");
		fflush(stdout);
		free(tok.str);
	}
	return (EXIT_SUCCESS);
}
