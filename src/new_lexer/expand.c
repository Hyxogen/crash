/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 10:52:43 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/02/22 14:39:29 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "new_lexer.h"
#include "parser.h"
#include "memory.h"

void
	lex_arithmetic(t_lexer *lex, t_tpart *part)
{
	t_lexer	lexer;

	part->id = lx_arithmetic;
	part->data = sh_safe_malloc(sizeof(t_token));
	part->quote = lex->quote;
	lex_init(&lexer);
	lexer.tok = part->data;
	lexer.prev = lex;
	lexer.id = lx_arithmetic;
	lexer.src = lex->src;
	lexer.end = NULL;
	lexer.here_flags = 0;
	lex_main(&lexer);
	/* TODO: destroy resources */
}


void
	lex_command(t_lexer *lex, t_tpart *part)
{
	t_lexer		lexer;
	t_parser	parser;
	t_snode		*node;

	lex_init(&lexer);
	lexer.prev = lex;
	lexer.id = lx_command;
	lexer.src = lex->src;
	lexer.end = NULL;
	lexer.here_flags = 0;
	pr_init(&parser);
	parser.lexer = &lexer;
	node = pr_parse(&parser);
	part->data = node;
	part->quote = lex->quote;
	/* TODO: destroy resources */
}

void
	lex_here(t_lexer *lex, t_token *tok, const char *end, int flags)
{
	t_lexer	lexer;

	lex_init(&lexer);
	lexer.prev = lex;
	lexer.id = lx_normal;
	lexer.src = lex->src;
	lexer.end = end;
	lexer.here_flags = flags;
	lexer.tok = tok;
	lex_main(&lexer);
	/* TODO: destroy resources */
}

void
	lex_normal(t_source *src, struct s_snode *node)
{
	t_parser	parser;
	t_lexer		lexer;

	lex_init(&lexer);
	lexer.prev = NULL;
	lexer.id = lx_normal;
	lexer.src = src;
	lexer.end = NULL;
	lexer.here_flags = 0;
	pr_init(&parser);
	*node = *pr_parse(&parser);
	/* TODO: destroy resources */
}

void
	lex_parameter(t_lexer *lex, t_tpart *part)
{
	(void) lex;
	(void) part;
	/* TODO: implement parameter expansion parser */
}
