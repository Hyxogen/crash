/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   arith_minus.c                                    -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:01 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:01 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long
	arith_minus(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs - rhs);
}

long
	arith_minus_eq(const char *str, long lhs, long rhs, long c)
{
	char	difference_str[32];
	long	difference;

	(void) c;
	difference = lhs - rhs;
	sh_ltoa(difference, difference_str, 32);
	sh_setenv(str, difference_str, 0);
	return (difference);
}
