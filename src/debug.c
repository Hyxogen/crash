#include "minishell.h"
#include "lexer.h"
#include "parser.h"

#include <unistd.h>
#include <stdlib.h>

static const char	*g_sx_names[] = {
	"sx_none",
	"sx_and",
	"sx_cmd_word",
	"sx_semicolon",
	"sx_separator_op",
	"sx_separator",
	"sx_and_if",
	"sx_or_if",
	"sx_pipe",
	"sx_newline",
	"sx_newline_list",
	"sx_word",
	"sx_assword",
	"sx_simple_cmd",
	"sx_pipe_sequence",
	"sx_pipeline",
	"sx_list",
	"sx_complete_cmd",
	"sx_and_or",
	"sx_linebreak",
	"sx_bang",
	"sx_io_file",
	"sx_less",
	"sx_lessand",
	"sx_great",
	"sx_greatand",
	"sx_dgreat",
	"sx_lessgreat",
	"sx_clobber",
	"sx_filename",
	"sx_io_redirect",
	"sx_io_number",
	"sx_cmd_suffix",
	"sx_cmd_prefix",
	"sx_cmd",
	"sx_term",
	"sx_compound_list",
	"sx_subshell",
	"sx_brace_group",
	"sx_compound_cmd",
	"sx_while_clause",
	"sx_until_clause",
	"sx_do_group",
	"sx_if_clause",
	"sx_else_part",
	"sx_function_def",
	"sx_function_body",
	"sx_function_name",
	"sx_case_clause",
	"sx_case_list",
	"sx_case_item",
	"sx_pattern",
	"sx_for_clause",
	"sx_for_name",
	"sx_wordlist",
	"sx_condition",
	"sx_elif_part",
	"sx_io_here",
	"sx_complete_cmdlst",
	"sx_io_redirect_list",
	"sx_ass_list"
};

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

static const char	*g_lx_names[] = {
	"lx_normal",
	"lx_command",
	"lx_parameter",
	"lx_arithmetic",
	"lx_backtick"
};

static const char	*g_sx_fnames[] = {
	"&",
	";",
	"!",
	"&&",
	"||",
	"\\n",
	"-"
};

void	print_node(t_snode *node, size_t depth);
void	print_token(t_token *tok, size_t depth);

void
	print_sflags(t_snode *node)
{
	int flag;

	flag = 1;
	while (flag <= flag_trim)
	{
		if (node->flags & flag)
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd((char *) g_sx_fnames[__builtin_ctz(node->flags & flag)], STDOUT_FILENO);
		}
		flag <<= 1;
	}
}

void
	print_tpart(t_tpart *part, size_t depth)
{
	size_t	i;

	i = 0;
	while (i < depth)
	{
		ft_putstr_fd((char*) "  ", STDOUT_FILENO);
		i += 1;
	}
	if (part->quote)
		ft_putstr_fd("(quoted) ", STDOUT_FILENO);
	ft_putstr_fd((char *) g_lx_names[part->id], STDOUT_FILENO);
	if (part->id == lx_normal || part->id == lx_backtick)
	{
		ft_putstr_fd((char *) ": ", STDOUT_FILENO);
		write(STDOUT_FILENO, part->data, part->len);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	else if (part->id == lx_command)
	{
		ft_putstr_fd((char *) ":\n", STDOUT_FILENO);
		print_node(part->data, depth + 1);
	}
	else if (part->id == lx_parameter || part->id == lx_arithmetic)
	{
		ft_putstr_fd((char *) ":\n", STDOUT_FILENO);
		print_token(part->data, depth + 1);
	}
}

void
	print_token(t_token *tok, size_t depth)
{
	size_t	i;

	i = 0;
	while (i < depth)
	{
		ft_putstr_fd((char*) "  ", STDOUT_FILENO);
		i += 1;
	}
	ft_putstr_fd((char *) g_tk_names[tok->id], STDOUT_FILENO);
	ft_putstr_fd((char *) ": ", STDOUT_FILENO);
	write(STDOUT_FILENO, tok->str, tok->len);
	ft_putchar_fd('\n', STDOUT_FILENO);
	i = 0;
	while (i < tok->count)
	{
		print_tpart(&tok->parts[i], depth + 1);
		i += 1;
	}
}

void
	print_node(t_snode *node, size_t depth)
{
	size_t	i;

	i = 0;
	while (i < depth)
	{
		ft_putstr_fd((char*) "  ", STDOUT_FILENO);
		i += 1;
	}
	ft_putstr_fd((char*) g_sx_names[node->type], STDOUT_FILENO);
	print_sflags(node);
	if (!node->childs_size && node->token && node->token->str)
	{
		ft_putchar_fd(':', STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		print_token(node->token, depth + 1);
	}
	else
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	if (node->here_content != NULL)
	{
		i = 0;
		while (i < depth + 1)
		{
			ft_putstr_fd((char*) "  ", STDOUT_FILENO);
			i += 1;
		}
		ft_putstr_fd((char*) "HERE CONTENT:\n", STDOUT_FILENO);
		print_token(node->here_content, depth + 2);
	}
	i = 0;
	while (i < node->childs_size)
	{
		print_node(node->childs[i], depth + 1);
		i += 1;
	}
}

t_snode
	*pr_parse(t_parser *pr)
{
	t_snode	*node;
	t_snode	*child;

	node = snode(sx_none);
	if (!pr_complete_cmd(pr, node))
	{
		node_destroy(node);
		return (NULL);
	}
	sh_assert(node->childs_size == 1);
	child = node->childs[0];
	node->childs_size = 0;
	node_destroy(node);
	return (child);
}

int
	main(void)
{
	t_input		in;
	t_source	src;
	t_lexer		lex;
	t_parser	pr;
	t_snode		*node;
	t_token		*tmp;

	input_new(&in, in_readline, NULL);
	src_init(&src, &in);
	lex_init(&lex);
	pr_init(&pr);
	lex.src = &src;
	pr.lexer = &lex;
	while (1)
	{
		if (pr.current_ret != 0)
		{
			tmp = pr.current;
			pr_next_token(&pr);
			token_destroy(tmp);
			free(tmp);
		}
		else
			pr_next_token(&pr);
		if (pr.current_ret == 0)
			break ;
		node = pr_parse(&pr);
		if (node != NULL)
			print_node(node, 0);
		node_destroy(node);
	}
	input_destroy(&in);
	pr_destroy(&pr);
}
