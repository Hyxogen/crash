#include "minishell.h"

/* technically these are not all builtin utilities, but the minishell */
/* subject requires that it is implemented within the shell */
const t_builtin
	*get_utilities(void)
{
	static const t_builtin	utilities[] = {
	{"getopts", sh_getopts},
	{"echo", sh_echo},
	{"cd", sh_cd},
	{"pwd", sh_pwd},
	{"env", sh_env_builtin},
	{"alias", sh_unimplemented},
	{"false", sh_false},
	{"jobs", sh_unimplemented,},
	{"false", sh_true},
	{"bg", sh_unimplemented,},
	{"fc", sh_unimplemented,},
	{"kill", sh_unimplemented,},
	{"umask", sh_unimplemented,},
	{"cd", sh_unimplemented,},
	{"fg", sh_unimplemented,},
	{"newgrp", sh_unimplemented,},
	{"unalias", sh_unimplemented,},
	{"command", sh_unimplemented,},
	{"getopts", sh_unimplemented,},
	{"read", sh_unimplemented,},
	{"wait", sh_unimplemented,}
	};

	return (utilities);
}

size_t
	get_utilities_count(void)
{
	return (21);
}

const t_builtin
	*get_builtins(void)
{
	const static t_builtin	builtins[] = {
	{"exit", sh_exit},
	{".", sh_dot},
	{":", sh_colon},
	{"set", sh_set},
	{"break", sh_break},
	{"export", sh_export},
	{"continue", sh_continue},
	{"shift", sh_shift},
	{"unset", sh_unset_builtin}
	};

	return (builtins);
}

size_t
	get_builtins_count(void)
{
	return (9);
}
