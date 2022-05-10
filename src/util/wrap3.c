/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   wrap3.c                                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:49 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:49 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <unistd.h>

int
	sh_chdir(const char *path)
{
	int	ret;

	ret = chdir(path);
	sh_check(ret >= 0, "chdir");
	return (ret);
}

char
	*sh_getcwd(void)
{
	char	*ret;

	ret = getcwd(NULL, 0);
	sh_check(ret != NULL, "getcwd");
	return (ret);
}
