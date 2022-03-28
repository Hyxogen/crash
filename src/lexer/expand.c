/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: csteenvo <csteenvo@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/25 16:22:44 by csteenvo      #+#    #+#                 */
/*   Updated: 2022/03/28 10:38:25 by dmeijer       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include "memory.h"

t_snode
	*lexpr_parse(t_parser *pr)
{
	t_snode	*node;
	t_snode	*child;

	node = snode(sx_none);
	pr_complete_cmdlst(pr, node);
	sh_assert(node->childs_size == 1);
	child = node->childs[0];
	node->childs_size = 0;
	node_destroy(node);
	return (child);
}

void
	lex_arithmetic(t_lexer *lex, t_tpart *part)
{
	t_lexer	lexer;

	part->id = lx_arithmetic;
	part->data = sh_safe_malloc(sizeof(t_token));
	part->quote = lex->quote;
	token_init(part->data);
	token_add_part(part->data, lx_normal);
	lex_init(&lexer);
	lexer.tok = part->data;
	lexer.tok->id = tk_word;
	lexer.prev = lex;
	lexer.id = lx_arithmetic;
	lexer.src = lex->src;
	lexer.end = NULL;
	lexer.here_flags = 0;
	if (lexer.src->cur == -1)
		lex_advance(&lexer);
	lex_main(&lexer);
	if (lex->error == 0)
		lex->error = lexer.error;
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
	if (lexer.src->cur == -1)
		lex_advance(&lexer);
	pr_init(&parser);
	parser.lexer = &lexer;
	pr_next_token(&parser);
	node = lexpr_parse(&parser);
	part->data = node;
	part->quote = lex->quote;
	pr_destroy(&parser);
	if (lex->error == 0)
		lex->error = lexer.error;
}

void
	lex_here(t_lexer *lex, t_token *tok, const char *end, int flags)
{
	t_lexer	lexer;

	lex_init(&lexer);
	lexer.prev = lex->prev;
	lexer.id = lx_normal;
	lexer.src = lex->src;
	lexer.end = end;
	lexer.here_flags = flags;
	lexer.tok = tok;
	token_init(lexer.tok);
	token_add_part(lexer.tok, lx_normal);
	lexer.tok->id = tk_word;
	if (lexer.src->cur == -1)
		lex_advance(&lexer);
	lex_main(&lexer);
	if (lex->error == 0)
		lex->error = lexer.error;
}

// NOTE: check errors if ever used
// void
// 	lex_normal(t_source *src, struct s_snode *node)
// {
// 	t_parser	parser;
// 	t_lexer		lexer;

// 	lex_init(&lexer);
// 	lexer.prev = NULL;
// 	lexer.id = lx_normal;
// 	lexer.src = src;
// 	lexer.end = NULL;
// 	lexer.here_flags = 0;
// 	if (lexer.src->cur == -1)
// 		lex_advance(&lexer);
// 	pr_init(&parser);
// 	*node = *pr_parse(&parser);
// 	pr_destroy(&parser);
// }

void
	lex_parameter(t_lexer *lex, t_tpart *part)
{
	t_lexer	lexer;

	part->id = lx_parameter;
	part->data = sh_safe_malloc(sizeof(t_token));
	part->quote = lex->quote;
	token_init(part->data);
	token_add_part(part->data, lx_normal);
	lex_init(&lexer);
	lexer.tok = part->data;
	lexer.tok->id = tk_word;
	lexer.prev = lex;
	lexer.id = lx_parameter;
	lexer.src = lex->src;
	lexer.end = NULL;
	lexer.here_flags = 0;
	if (lexer.src->cur == -1)
		lex_advance(&lexer);
	lex_main(&lexer);
	if (lex->error == 0)
		lex->error = lexer.error;
}
