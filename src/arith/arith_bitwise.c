/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   arith_bitwise.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:55 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:55 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

long
	arith_bitwise_or(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs | rhs);
}

long
	arith_bitwise_and(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs & rhs);
}

long
	arith_bitwise_xor(const char *ptr, long lhs, long rhs, long c)
{
	(void) ptr;
	(void) c;
	return (lhs ^ rhs);
}
