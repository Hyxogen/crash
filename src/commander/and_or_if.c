#include "commander.h"

#include "minishell.h"
/*
$ echo Well && echo hello && echo there
sx_complete_cmd
  sx_compound_list
    sx_and_if
      sx_pipeline
        sx_simple_cmd
          sx_wordlist
            sx_word:
              tk_word: echo
                lx_normal: echo
            sx_word:
              tk_word: Well
                lx_normal: Well
          sx_io_redirect_list
          sx_ass_list
      sx_and_if
        sx_pipeline
          sx_simple_cmd
            sx_wordlist
              sx_word:
                tk_word: echo
                  lx_normal: echo
              sx_word:
                tk_word: hello
                  lx_normal: hello
            sx_io_redirect_list
            sx_ass_list
        sx_pipeline
          sx_simple_cmd
            sx_wordlist
              sx_word:
                tk_word: echo
                  lx_normal: echo
              sx_word:
                tk_word: there
                  lx_normal: there
            sx_io_redirect_list
            sx_ass_list
*/

int
	cm_and_if(const t_snode *node, const int io[3])
{
	int	return_code;

	sh_assert(node->childs_size >= 2);
	return_code = commandeer(node->childs[0], io);
	if (return_code == 0)
		return (commandeer(node->childs[1], io));
	return (return_code);
}

int
	cm_or_if(const t_snode *node, const int io[3])
{
	int	return_code;

	sh_assert(node->childs_size >= 2);
	return_code = commandeer(node->childs[0], io);
	if (return_code != 0)
		return (commandeer(node->childs[1], io));
	return (return_code);
}
