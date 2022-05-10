/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ltoa.c                                           -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:40 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:40 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>

#define SH_LONG_MIN_STR "-9223372036854775808"
#define SH_ZERO_STR "0"

static size_t
	sh_ltoa_normal(long value, char *str, size_t len)
{
	size_t	nwritten;

	if (len == 0 || value == 0)
		return (0);
	if (value < 0)
	{
		str[0] = '-';
		return (sh_ltoa_normal(-value, str + 1, len - 1) + 1);
	}
	nwritten = sh_ltoa_normal(value / 10, str, len);
	str[nwritten] = '0' + (value % 10);
	if (nwritten < len)
		str[nwritten + 1] = '\0';
	else
		str[nwritten] = '\0';
	return (nwritten + 1);
}

size_t
	sh_ltoa(long value, char *str, size_t len)
{
	if (value == LONG_MIN)
		return (ft_strlcpy(str, SH_LONG_MIN_STR, len));
	else if (value == 0)
		return (ft_strlcpy(str, SH_ZERO_STR, len));
	else
		return (sh_ltoa_normal(value, str, len));
}
