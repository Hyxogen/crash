#include "commander.h"

#include <string.h>


#include <stdio.h>
/*
$ if true; then echo Hallo; elif false; then echo Ok?; elif false; then echo Huh?; else echo KO; fi
sx_list
  sx_term
    sx_pipe_sequence
      sx_if_clause
        sx_compound_list
          sx_term ;
            sx_pipe_sequence
              sx_simple_cmd
                sx_wordlist
                  sx_word:
                    tk_word: true
                      lx_normal: true
                sx_io_redirect_list
                sx_ass_list
        sx_compound_list
          sx_term ;
            sx_pipe_sequence
              sx_simple_cmd
                sx_wordlist
                  sx_word:
                    tk_word: echo
                      lx_normal: echo
                  sx_word:
                    tk_word: Hallo
                      lx_normal: Hallo
                sx_io_redirect_list
                sx_ass_list
        sx_if_clause
          sx_compound_list
            sx_term ;
              sx_pipe_sequence
                sx_simple_cmd
                  sx_wordlist
                    sx_word:
                      tk_word: false
                        lx_normal: false
                  sx_io_redirect_list
                  sx_ass_list
          sx_compound_list
            sx_term ;
              sx_pipe_sequence
                sx_simple_cmd
                  sx_wordlist
                    sx_word:
                      tk_word: echo
                        lx_normal: echo
                    sx_word:
                      tk_word: Ok?
                        lx_normal: Ok?
                  sx_io_redirect_list
                  sx_ass_list
          sx_if_clause
            sx_compound_list
              sx_term ;
                sx_pipe_sequence
                  sx_simple_cmd
                    sx_wordlist
                      sx_word:
                        tk_word: false
                          lx_normal: false
                    sx_io_redirect_list
                    sx_ass_list
            sx_compound_list
              sx_term ;
                sx_pipe_sequence
                  sx_simple_cmd
                    sx_wordlist
                      sx_word:
                        tk_word: echo
                          lx_normal: echo
                      sx_word:
                        tk_word: Huh?
                          lx_normal: Huh?
                    sx_io_redirect_list
                    sx_ass_list
            sx_compound_list
              sx_term ;
                sx_pipe_sequence
                  sx_simple_cmd
                    sx_wordlist
                      sx_word:
                        tk_word: echo
                          lx_normal: echo
                      sx_word:
                        tk_word: KO
                          lx_normal: KO
                    sx_io_redirect_list
                    sx_ass_list
        sx_io_redirect_list
*/

/* TODO setup redirects */
pid_t
	cm_if_clause(t_minishell *sh, t_snode *ifnode, const int io[3])
{
	int	rc;

	sh_assert(ifnode->type == sx_if_clause);
	rc = commandeer(sh, ifnode->childs[0], io);
	if (!rc)
		return (cm_convert_retcode(commandeer(sh, ifnode->childs[1], io)));
	if (ifnode->childs_size >= 3 && ifnode->childs[2]->type == sx_if_clause)
		return (cm_if_clause(sh, ifnode->childs[2], io));
	else if (ifnode->childs_size >= 3)
		return (cm_convert_retcode(commandeer(sh, ifnode->childs[2], io)));
	return (cm_convert_retcode(0));
}
