/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   add.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:15 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:15 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

#include "memory.h"

t_tpart
	*token_add_part(t_lexer *lex, t_lexer_id id, int quote)
{
	t_tpart	part;
	t_token	*tok;

	tok = lex->tok;
	part.id = id;
	part.data = NULL;
	part.len = 0;
	part.quote = quote != 0;
	tok->parts = sh_safe_realloc(tok->parts, tok->count * sizeof(part),
			(tok->count + 1) * sizeof(part));
	tok->parts[tok->count] = part;
	tok->count += 1;
	lex->new_part = 0;
	lex->empty_quote = 0;
	return (&tok->parts[tok->count - 1]);
}
