/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   arith_multiply.c                                 -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:02 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:02 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long
	arith_multiply(const char *str, long lhs, long rhs, long c)
{
	(void) str;
	(void) c;
	return (lhs * rhs);
}

long
	arith_multiple_eq(const char *str, long lhs, long rhs, long c)
{
	char	product_str[32];
	long	product;

	(void) c;
	product = lhs * rhs;
	sh_ltoa(product, product_str, 32);
	sh_setenv(str, product_str, 0);
	return (product);
}
