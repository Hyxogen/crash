/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   arith_condition.c                                -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:57 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:57 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "commander.h"

long
	arith_logical_and(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs && rhs);
}

long
	arith_logical_or(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs || rhs);
}
