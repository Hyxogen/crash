#include "commander.h"
#include "minishell.h"
#include "memory.h"
#include <unistd.h>

int
	arith_lex_op1(t_arith_lexer *lex, int c)
{
	if (*lex->str == c)
	{
		lex->str += 1;
		lex->tok->size += 1;
		return (1);
	}
	return (0);
}

t_arith_token_id
	arith_lex_op2(t_arith_lexer *lex, int c, t_arith_token_id one,
		t_arith_token_id two)
{
	if (arith_lex_op1(lex, c))
		return (two);
	return (one);
}

t_arith_token_id
	arith_lex_op_less_great(t_arith_lexer *lex, int c)
{
	if (c == '<')
	{
		if (arith_lex_op1(lex, '<'))
			return (arith_lex_op2(lex, '=', ar_op_less_less, ar_op_less_less_eq));
		return (arith_lex_op2(lex, '=', ar_op_less, ar_op_less_eq));
	}
	else
	{
		if (arith_lex_op1(lex, '>'))
			return (arith_lex_op2(lex, '=', ar_op_great_great, ar_op_great_great_eq));
		return (arith_lex_op2(lex, '=', ar_op_great, ar_op_great_eq));
	}
}

t_arith_token_id
	arith_lex_op_amp_pipe(t_arith_lexer *lex, int c)
{
	if (c == '&')
	{
		if (arith_lex_op1(lex, '&'))
			return (ar_op_amp_amp);
		return (arith_lex_op2(lex, '=', ar_op_amp, ar_op_amp_eq));
	}
	else
	{
		if (arith_lex_op1(lex, '|'))
			return (ar_op_pipe_pipe);
		return (arith_lex_op2(lex, '=', ar_op_pipe, ar_op_pipe_eq));
	}
}

t_arith_token_id
	arith_lex_op_binary(t_arith_lexer *lex)
{
	if (arith_lex_op1(lex, '+'))
		return (arith_lex_op2(lex, '=', ar_op_plus, ar_op_plus_eq));
	if (arith_lex_op1(lex, '-'))
		return (arith_lex_op2(lex, '=', ar_op_minus, ar_op_minus_eq));
	if (arith_lex_op1(lex, '*'))
		return (arith_lex_op2(lex, '=', ar_op_star, ar_op_star_eq));
	if (arith_lex_op1(lex, '/'))
		return (arith_lex_op2(lex, '=', ar_op_slash, ar_op_slash_eq));
	if (arith_lex_op1(lex, '%'))
		return (arith_lex_op2(lex, '=', ar_op_perc, ar_op_perc_eq));
	if (arith_lex_op1(lex, '&'))
		return (arith_lex_op_amp_pipe(lex, '&'));
	if (arith_lex_op1(lex, '^'))
		return (arith_lex_op2(lex, '=', ar_op_caret, ar_op_caret_eq));
	if (arith_lex_op1(lex, '|'))
		return (arith_lex_op_amp_pipe(lex, '|'));
	return (ar_tk_null);
}

t_arith_token_id
	arith_lex_op(t_arith_lexer *lex)
{
	if (arith_lex_op1(lex, '('))
		return (ar_op_lparen);
	if (arith_lex_op1(lex, ')'))
		return (ar_op_rparen);
	if (arith_lex_op1(lex, '<'))
		return (arith_lex_op_less_great(lex, '<'));
	if (arith_lex_op1(lex, '>'))
		return (arith_lex_op_less_great(lex, '>'));
	if (arith_lex_op1(lex, '?'))
		return (ar_op_quest);
	if (arith_lex_op1(lex, ':'))
		return (ar_op_colon);
	if (arith_lex_op1(lex, '='))
		return (arith_lex_op2(lex, '=', ar_op_eq, ar_op_eq_eq));
	if (arith_lex_op1(lex, '!'))
		return (arith_lex_op2(lex, '=', ar_op_excl, ar_op_excl_eq));
	if (arith_lex_op1(lex, '~'))
		return (ar_op_tilde);
	return (arith_lex_op_binary(lex));
}

size_t
	arith_lex_ident(const char *str)
{
	size_t	i;

	if (*str != '_' && !ft_isalpha(*str))
		return (0);
	i = 1;
	while (str[i] == '_' || ft_isalnum(str[i]))
		i += 1;
	return (i);
}

size_t
	arith_lex_const(const char *str)
{
	size_t	i;

	if (!ft_isdigit(*str))
		return (0);
	i = 1;
	while (str[i] == '_' || ft_isalnum(str[i]))
		i += 1;
	return (i);
}

int
	arith_lex(t_arith_lexer *lex, t_arith_token *tok)
{
	while (ft_isspace(*lex->str))
		lex->str += 1;
	if (*lex->str == '\0')
	{
		tok->id = ar_tk_eof;
		return (0);
	}
	lex->tok = tok;
	tok->str = lex->str;
	tok->size = 0;
	tok->id = arith_lex_op(lex);
	if (tok->id != ar_tk_null)
		return (1);
	tok->size = arith_lex_ident(lex->str);
	if (tok->size != 0)
		tok->id = ar_tk_ident;
	else
	{
		tok->size = arith_lex_const(lex->str);
		if (tok->size != 0)
			tok->id = ar_tk_const;
	}
	lex->str += tok->size;
	if (tok->id != ar_tk_null)
		return (1);
	return (-1);
}

int
	arith_pr_assume(t_arith_parser *pr, t_arith_token_id id)
{
	if (pr->next.id == id)
	{
		arith_lex(pr->lex, &pr->next);
		return (1);
	}
	return (0);
}

int	arith_pr_main(t_arith_parser *pr, t_arith_node *parent);

t_arith_node
	*arith_node(t_arith_node_id id)
{
	t_arith_node	*node;

	node = sh_safe_malloc(sizeof(*node));
	node->id = id;
	node->children = NULL;
	node->children_size = 0;
	return (node);
}

void
	arith_node_add(t_arith_node *parent, t_arith_node *node)
{
	parent->children = sh_safe_realloc(parent->children,
		sizeof(*parent->children) * parent->children_size,
		sizeof(*parent->children) * (parent->children_size + 1));
	parent->children[parent->children_size] = node;
	parent->children_size += 1;
}

void
	arith_node_destroy(t_arith_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->children_size)
	{
		arith_node_destroy(node->children[i]);
		i += 1;
	}
	free(node->children);
	free(node);
}

int
	arith_pr_paren(t_arith_parser *pr, t_arith_node *parent)
{
	t_arith_node	*node;

	node = arith_node(ar_sx_primary);
	if (arith_pr_primary(pr, node)
		&& arith_pr_assume(pr, ar_op_rparen))
	{
		arith_node_add(parent, node);
		return (1);
	}
	arith_node_destroy(node);
	return (0);
}

int
	arith_pr_primary(t_arith_parser *pr, t_arith_node *parent)
{
	if (arith_pr_assume(pr, ar_op_lparen))
		return (arith_pr_paren(pr, parent));
	return (0);
}

int
	expand_arith(t_expand *exp, t_token *token)
{
	char			*str;
	t_arith_lexer	lex;
	t_arith_token	tok;

	str = cm_expand_str(token, NULL, ' ');
	if (str == NULL)
		return (-1);
	ft_fprintf(STDERR_FILENO, "arithmetic expansion: %s\n", str);
	lex.str = str;
	lex.tok = &tok;
	while (1)
	{
		if (arith_lex(&lex, &tok) != 1)
			break ;
		ft_fprintf(STDERR_FILENO, "arithmetic token: %d\n", tok.id);
	}
	// TODO: implement
	return (-1);
}
