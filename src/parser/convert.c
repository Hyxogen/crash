/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   convert.c                                        -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:26 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:26 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <libft.h>

int
	pr_is_name(const char *str, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i += 1;
	}
	return (1);
}

/* rule 1 */
/* NOTE: If for some reason you would want more keywords,
	you must increase the number in the while loop to the correct count*/
int
	pr_convert_reserved(t_parser *pr, t_token *token)
{
	size_t	i;

	i = 0;
	while (i < 16)
	{
		if (pr_convert_keyword(pr, token, kw_if + i))
			return (1);
		i += 1;
	}
	return (0);
}

/* rule 4, 6 */
int
	pr_convert_keyword(t_parser *pr, t_token *token, t_token_id id)
{
	size_t	len;

	(void) pr;
	if (token != NULL && token->id == id)
		return (1);
	if (token == NULL || token->id != tk_word)
		return (0);
	len = ft_strlen(pr_get_keywords()[id - kw_if]);
	SH_ASSERT(token->str != NULL);
	if (ft_memcmp(pr_get_keywords()[id - kw_if], token->str, len + 1) == 0)
	{
		token->id = id;
		return (1);
	}
	return (0);
}

/* rule 5 */
int
	pr_convert_name(t_parser *pr, t_token *token)
{
	(void) pr;
	if (token == NULL || token->id != tk_word)
		return (0);
	if (pr_is_name(token->str, token->len))
	{
		token->id = tk_name;
		return (1);
	}
	return (0);
}

/* rule 7 */
int
	pr_convert_ass(t_parser *pr, t_token *token, int first)
{
	char	*end;

	if (token == NULL || token->id != tk_word)
		return (0);
	end = ft_memchr(token->str, '=', token->len);
	if (end != NULL)
	{
		if (token->str[0] == '=')
			return (0);
		if (!pr_is_name(token->str, end - token->str))
			return (0);
		token->id = tk_assword;
		return (1);
	}
	else if (first)
	{
		pr_convert_reserved(pr, token);
	}
	return (0);
}
