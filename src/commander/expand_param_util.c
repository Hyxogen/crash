/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand_param_util.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:44 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:44 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

#include <stdlib.h>

size_t
	expand_key_length(const char *param)
{
	size_t	i;

	if (param[0] == '*'
		|| param[0] == '@'
		|| param[0] == '#'
		|| param[0] == '?'
		|| param[0] == '-'
		|| param[0] == '$'
		|| param[0] == '!')
		return (1);
	i = 0;
	if (ft_isdigit(param[0]))
		while (ft_isdigit(param[i]))
			i += 1;
	else
		while (ft_isalnum(param[i]) || param[i] == '_')
			i += 1;
	return (i);
}

int
	expand_empty(t_expand *exp, int empty_is_null)
{
	if (exp->count == 0 || exp->parts[0].str[0] == NULL)
		return (1);
	if (exp->parts[0].str[0][0] == '\0'
		&& exp->parts[0].str[1] == NULL
		&& empty_is_null)
		return (1);
	return (0);
}

size_t
	expand_length(t_expand *exp)
{
	size_t	i;
	int		c;
	size_t	n;

	if (exp->count == 0)
		return (0);
	i = 0;
	n = 0;
	if (exp->parts[0].array)
		while (exp->parts[0].str[n] != NULL)
			n += 1;
	else
	{
		while (exp->parts[0].str[0][i] != '\0')
		{
			c = exp->parts[0].str[0][i];
			n += ((c & 0x80) == 0 || (c & 0xC0) == 0xC0);
			i += 1;
		}
	}
	return (n);
}

int
	expand_error(t_param_ctx *ctx, size_t i)
{
	char	*str;

	if (((char *) ctx->token->parts[0].data)[i] == '\0')
		sh_err2(ctx->key, "parameter not set");
	else
	{
		ctx->token->parts[0].data = (char *) ctx->token->parts[0].data + i;
		str = cm_expand_str(ctx->token, NULL, ' ', 1);
		ctx->token->parts[0].data = (char *) ctx->token->parts[0].data - i;
		if (str == NULL)
			return (-1);
		sh_err2(ctx->key, str);
		free(str);
	}
	if (!sh()->interactive)
		exit(EXIT_FAILURE);
	return (-1);
}
