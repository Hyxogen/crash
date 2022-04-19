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
