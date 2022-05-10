/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   atol.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:35 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:35 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

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

int
	sh_arith_digit(int base, int c, long *v)
{
	const char	*charset = "0123456789ABCDEF";
	int			i;

	i = 0;
	while (i < base)
	{
		if (charset[i] == ft_toupper(c))
		{
			*v = *v * base + i;
			return (0);
		}
		i += 1;
	}
	return (-1);
}

int
	sh_arith_atol(const char *str, long *v)
{
	int	base;

	*v = 0;
	base = 10;
	if (*str == '0')
	{
		base = 8;
		str += 1;
		if (*str == '\0')
			return (0);
		if (*str == 'x' || *str == 'X')
			base = 16;
		str += (*str == 'x' || *str == 'X' || *str == 'o' || *str == 'O');
	}
	if (sh_arith_digit(base, *str, v) < 0)
		return (-1);
	str += 1;
	while (sh_arith_digit(base, *str, v) == 0)
		str += 1;
	return (-(*str != '\0'));
}
