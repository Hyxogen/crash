#include "minishell.h"
#include "commander.h"
#include "memory.h"
#include "libft.h"

#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

void	cm_wildcard_int(char *prefix, char *rest, int *info, char ***out);

void
	cm_wildcard_dir(char *prefix, char *rest, int *info, t_wildcard_ctx *ctx)
{
	DIR				*dir;
	t_pattern_node	*pattern;
	struct dirent	*ent;
	char			*tmp;

	if (prefix[0] == '\0')
		dir = opendir(".");
	else
		dir = opendir(prefix);
	if (dir == NULL)
		return ;
	pattern = pattern_compile(rest, info);
	ent = readdir(dir);
	while (ent != NULL)
	{
		if (pattern_match(ent->d_name, pattern, 1))
		{
			tmp = cm_wildcard_join(prefix, ent->d_name, &"/"[!ctx->slash]);
			cm_wildcard_int(tmp, rest + ctx->i + ctx->slash,
				info + ctx->i + 1, ctx->out);
			ctx->has_match = (free(tmp), 1);
		}
		ent = readdir(dir);
	}
	(void) (pattern_destroy(pattern), closedir(dir));
}

void
	cm_wildcard_int(char *prefix, char *rest, int *info, char ***out)
{
	t_wildcard_ctx	ctx;
	char			*tmp;

	if (*rest == '\0')
	{
		cm_wildcard_add(prefix, out);
		return ;
	}
	ctx.out = out;
	ctx.i = 0;
	while (rest[ctx.i] != '/' && rest[ctx.i] != '\0')
		ctx.i += 1;
	ctx.slash = rest[ctx.i] == '/';
	rest[ctx.i] = '\0';
	ctx.has_match = 0;
	cm_wildcard_dir(prefix, rest, info, &ctx);
	if (!ctx.has_match)
	{
		tmp = cm_wildcard_join(prefix, rest, &"/"[!ctx.slash]);
		cm_wildcard_int(tmp, rest + ctx.i + ctx.slash, info + ctx.i + 1, out);
		free(tmp);
	}
}

void
	cm_wildcard(const char *path, int *info, char ***out)
{
	int		root;
	char	*prefix;
	char	*tmp;

	root = path[0] == '/';
	prefix = ft_strndup(path, root);
	tmp = ft_strdup(path);
	cm_wildcard_int(prefix, tmp + root, info + root, out);
	free(prefix);
	free(tmp);
}

char
	**cm_wildcard_expand(const t_token *token)
{
	char	**list;
	int		**info;
	size_t	i;
	char	**out;

	list = cm_expand(token, &info, 1);
	if (list == NULL)
		return (NULL);
	out = sh_safe_malloc(sizeof(*out));
	out[0] = NULL;
	i = 0;
	while (list[i] != NULL)
	{
		cm_wildcard(list[i], info[i], &out);
		i += 1;
	}
	i = 0;
	while (list[i] != NULL)
	{
		free(info[i]);
		i += 1;
	}
	free(info);
	sh_strlst_clear(list);
	return (out);
}
