/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   export.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:59:13 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:59:13 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "commander.h"

void
	dump_export(const char *value)
{
	ft_putchar_fd('"', sh()->io[SH_STDOUT_INDEX]);
	while (*value)
	{
		if (ft_strchr("\\\"`", *value) != NULL)
			ft_putchar_fd('\\', sh()->io[SH_STDOUT_INDEX]);
		ft_putchar_fd(*value, sh()->io[SH_STDOUT_INDEX]);
		value += 1;
	}
	ft_putchar_fd('"', sh()->io[SH_STDOUT_INDEX]);
}

void
	dump_exports(void)
{
	size_t	i;

	i = 0;
	while (i < sh()->vars_size)
	{
		if (sh()->vars[i].attr & SH_ENV_EXPORT)
		{
			ft_putstr_fd("export ", sh()->io[SH_STDOUT_INDEX]);
			ft_putstr_fd(sh()->vars[i].key, sh()->io[SH_STDOUT_INDEX]);
			if (sh()->vars[i].value != NULL)
			{
				ft_putchar_fd('=', sh()->io[SH_STDOUT_INDEX]);
				dump_export(sh()->vars[i].value);
			}
			ft_putchar_fd('\n', sh()->io[SH_STDOUT_INDEX]);
		}
		i += 1;
	}
}

int
	sh_export(int argc, char **argv)
{
	t_envvar	*var;
	int			i;

	(void) argc;
	if (argc == 1)
		dump_exports();
	else
	{
		i = 1;
		while (i < argc)
		{
			if (ft_strchr(argv[i], '=') != NULL)
				var = _do_assignment(argv[i], 0);
			else
				var = sh_getenv_int(argv[i], 1);
			if (var != NULL)
				var->attr |= SH_ENV_EXPORT;
			i += 1;
		}
	}
	return (0);
}
