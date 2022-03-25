/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   add.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:22:37 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:22:37 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

#include "memory.h"

t_tpart
	*token_add_part(t_token *tok, t_lexer_id id)
{
	t_tpart	part;

	part.id = id;
	part.data = NULL;
	part.len = 0;
	part.quote = 0;
	tok->parts = sh_safe_realloc(tok->parts, tok->count * sizeof(part),
			(tok->count + 1) * sizeof(part));
	tok->parts[tok->count] = part;
	tok->count += 1;
	return (&tok->parts[tok->count - 1]);
}
