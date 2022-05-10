/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   basename.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:36 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:36 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char
	*sh_basename(char *str)
{
	char	*last_slash;

	last_slash = ft_strrchr(str, '/');
	if (last_slash == NULL)
		return (ft_strdup(str));
	return (ft_strdup(last_slash + 1));
}
