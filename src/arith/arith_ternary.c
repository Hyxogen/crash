/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   arith_ternary.c                                  -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:04 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:04 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "commander.h"

long
	arith_ternary(const char *str, long lhs, long rhs, long super_rhs)
{
	(void) str;
	if (lhs)
		return (rhs);
	return (super_rhs);
}
