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
