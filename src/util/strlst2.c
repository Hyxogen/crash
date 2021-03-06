/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   strlst2.c                                        -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:45 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:45 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

size_t
	sh_strlst_strsize(char **strs, int delim)
{
	size_t	tot_len;
	size_t	index;

	index = 0;
	tot_len = 0;
	while (strs[index])
	{
		tot_len
			+= ft_strlen(strs[index]) + (delim != '\0' && &strs[index] != strs);
		index++;
	}
	return (tot_len);
}
