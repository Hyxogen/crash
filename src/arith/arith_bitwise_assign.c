/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   arith_bitwise_assign.c                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:56 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:56 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "commander.h"
#include "minishell.h"

long
	arith_bitwise_or_eq(const char *str, long lhs, long rhs, long c)
{
	char	result_str[32];
	long	result;

	(void) c;
	result = lhs | rhs;
	sh_ltoa(result, result_str, 32);
	sh_setenv(str, result_str, 0);
	return (result);
}

long
	arith_bitwise_and_eq(const char *str, long lhs, long rhs, long c)
{
	char	result_str[32];
	long	result;

	(void) c;
	result = lhs & rhs;
	sh_ltoa(result, result_str, 32);
	sh_setenv(str, result_str, 0);
	return (result);
}

long
	arith_bitwise_xor_eq(const char *str, long lhs, long rhs, long c)
{
	char	result_str[32];
	long	result;

	(void) c;
	result = lhs ^ rhs;
	sh_ltoa(result, result_str, 32);
	sh_setenv(str, result_str, 0);
	return (result);
}
