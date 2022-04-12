#include <libft.h>
#include <limits.h>

int
	sh_atol(const char *str, long *v)
{
	int		s;

	while ((*str >= 9 && *str <= 13) || *str == 32)
		str += 1;
	s = 1;
	if (*str == '-')
		s = -s;
	if (*str == '-' || *str == '+')
		str += 1;
	*v = 0;
	if (*str == '\0')
		return (-1);
	while (ft_isdigit(*str))
	{
		if (*v > LONG_MAX / 10 || (s > 0 && *v * 10 > LONG_MAX - (*str - '0')))
			return (-1);
		if (*v < LONG_MIN / 10 || (s < 0 && *v * 10 < LONG_MIN + (*str - '0')))
			return (-1);
		*v = *v * 10 + (*str - '0') * s;
		str += 1;
	}
	if (*str != '\0')
		return (-1);
	return (0);
}
