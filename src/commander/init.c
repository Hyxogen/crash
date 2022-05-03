#include "minishell.h"
#include "commander.h"
#include "parser.h"
#include "memory.h"
#include <unistd.h>
#include <libgen.h>
#include <stdlib.h>

static const t_builtin builtins[] = {
	{ "exit", sh_exit },
	{ ".", sh_dot },
	{ ":", sh_colon },
	{ "set", sh_set },
	{ "break", sh_break },
	{ "export", sh_export },
	{ "continue", sh_continue },
	{ "shift", sh_shift }
};

static const t_builtin utilities[] = {
	{ "getopts", sh_getopts },
	/* technically echo is not a builtin utility, but the minishell subject */
	/* requires that it is implemented within the shell */
	{ "echo", sh_echo }
};

void
	sh_init(char **argv, char **env)
{
	char	*tmp;

	sh_env_init(env);
	tmp = getcwd(NULL, 0);
	sh()->self = sh_join_path(tmp, argv[0]);
	free(tmp);
	sh()->builtins = builtins;
	sh()->builtins_size = sizeof(builtins) / sizeof(builtins[0]);
	sh()->functions = NULL;
	sh()->functions_size = 0;
	sh()->utilities = utilities;
	sh()->utilities_size = sizeof(utilities) / sizeof(utilities[0]);
	sh()->args = argv; // TODO: should this be on the heap?
	sh()->interactive = 1; // TODO: check if interactive
	sh()->io[SH_STDIN_INDEX] = STDIN_FILENO;
	sh()->io[SH_STDOUT_INDEX] = STDOUT_FILENO;
	sh()->io[SH_STDERR_INDEX] = STDERR_FILENO;
	tmp = ft_strdup(argv[0]);
	sh()->name = basename(tmp); // TODO: can't use basename
	free(tmp);
	sh_setenv("OPTIND", "1", 0);
	sh_setenv("PS1", "$ ", 0);
	sh_setenv("PS2", "> ", 0);
	sh_setenv("PS4", "+ ", 0);
}

void
	sh_destroy(void)
{
	size_t	i;

	sh_env_destroy();
	i = 0;
	while (i < sh()->functions_size)
	{
		free(sh()->functions[i].key);
		node_destroy(sh()->functions[i].body);
		i++;
	}
	free(sh()->self);
	free(sh()->functions);
	free(sh()->name);
}

static t_function
	*sh_get_function(const char *key)
{
	size_t	index;

	index = 0;
	while (index < sh()->functions_size)
	{
		if (!ft_strcmp(sh()->functions[index].key, key))
			return (&sh()->functions[index]);
		index++;
	}
	return (NULL);
}

void
	sh_add_function(const char *key, t_snode *body)
{
	t_function	*entry;

	entry = sh_get_function(key);
	if (entry == NULL)
	{
		sh()->functions = sh_safe_realloc(sh()->functions,
			sizeof(*sh()->functions) * sh()->functions_size,
			sizeof(*sh()->functions) * (sh()->functions_size + 1));
		entry = &sh()->functions[sh()->functions_size];
		entry->key = NULL;
		entry->body = NULL;
		sh()->functions_size += 1;
	}
	/* TODO free previous entry */
	entry->key = ft_strdup(key);
	entry->body = sh_safe_malloc(sizeof(*body));
	node_move(entry->body, body);
	// sh()->functions[sh()->functions_size].body = body; // TODO: duplicate body?
}
