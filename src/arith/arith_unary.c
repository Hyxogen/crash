#include "minishell.h"
#include "commander.h"

long
	arith_identity(const char *str, long lhs, long b, long c)
{
	(void) str;
	(void) b;
	(void) c;
	return (lhs);
}

long
	arith_negate(const char *str, long lhs, long b, long c)
{
	(void) str;
	(void) b;
	(void) c;
	return (-lhs);
}

long
	arith_logical_not(const char *str, long lhs, long b, long c)
{
	(void) str;
	(void) b;
	(void) c;
	return (!lhs);
}

long
	arith_bitwise_not(const char *str, long lhs, long b, long c)
{
	(void) str;
	(void) b;
	(void) c;
	return (~lhs);
}
