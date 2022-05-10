/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   err.c                                            -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:38 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:38 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "commander.h"
#include "ft_printf.h"
#include <stdlib.h>

void
	sh_err1(const char *s1)
{
	ft_fprintf(sh()->io[SH_STDERR_INDEX], "%s: %s\n", sh()->name, s1);
}

void
	sh_err2(const char *s1, const char *s2)
{
	ft_fprintf(sh()->io[SH_STDERR_INDEX], "%s: %s: %s\n", sh()->name, s1, s2);
}

void
	sh_err3(const char *s1, const char *s2, const char *s3)
{
	ft_fprintf(sh()->io[SH_STDERR_INDEX],
		"%s: %s: %s: %s\n", sh()->name, s1, s2, s3);
}
