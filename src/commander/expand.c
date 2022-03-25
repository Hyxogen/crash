/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   expand.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:19 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:19 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "libft.h"

/* TODO properly expand token */
const char
	*cm_expand(t_token *token)
{
	return ft_strdup(token->str);
}
