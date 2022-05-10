/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   arith_equality.c                                 -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:58 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:58 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"

long
	arith_equal(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs == rhs);
}

long
	arith_not_equal(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs != rhs);
}
