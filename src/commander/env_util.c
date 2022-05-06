#include "minishell.h"

#include <libft.h>
#include <stdlib.h>

void
	sh_env_init(char **env)
{
	char		*key;
	char		*value;
	size_t		i;
	t_envvar	*var;

	i = 0;
	sh()->vars = NULL;
	sh()->vars_size = 0;
	while (env[i] != NULL)
	{
		sh_split2(env[i], '=', &key, &value);
		var = sh_setenv(key, value, 0);
		var->attr |= SH_ENV_EXPORT;
		free(key);
		free(value);
		i += 1;
	}
}

void
	sh_env_destroy(void)
{
	size_t	i;

	i = 0;
	while (i < sh()->vars_size)
	{
		free(sh()->vars[i].value);
		free(sh()->vars[i].tmp_value);
		free(sh()->vars[i].key);
		i += 1;
	}
	free(sh()->vars);
}
