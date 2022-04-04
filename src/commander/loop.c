#include "commander.h"

/*
$ for name in 1 2 3 4; do
> echo $name
> done
sx_list
  sx_term
    sx_pipe_sequence
      sx_for_clause:
        tk_name: name
          lx_normal: name
        sx_wordlist
          sx_word:
            tk_word: 1
              lx_normal: 1
          sx_word:
            tk_word: 2
              lx_normal: 2
          sx_word:
            tk_word: 3
              lx_normal: 3
          sx_word:
            tk_word: 4
              lx_normal: 4
        sx_compound_list
          sx_term \n
            sx_pipe_sequence
              sx_simple_cmd
                sx_wordlist
                  sx_word:
                    tk_word: echo
                      lx_normal: echo
                  sx_word:
                    tk_word: $name
                      lx_normal: 
                      lx_parameter:
                        tk_word: name
                      lx_normal: 
                sx_io_redirect_list
                sx_ass_list
        sx_io_redirect_list
*/

/* TODO implement no wordlist */
/* TODO setup redirects */
pid_t
	cm_for_clause(t_minishell *sh, t_snode *fornode, const int io[3])
{
	char	*name;
	char	**list;
	int		rc;
	t_snode	*do_node;

	name = fornode->token.str;
	list = cm_word_list_to_array(sh, fornode->childs[0]);
	do_node = fornode->childs[1];
	while (*list)
	{
		sh_setenv(sh, name, *list, 0);
		rc = commandeer(sh, do_node, io);
		list += 1;
	}
	sh_strlst_clear(list);
	return (cm_convert_retcode(rc));
}
