#include "commander.h"

#include <ft_printf.h>
#include <unistd.h>
#include <stdlib.h>

/*
sx_list
  sx_term
    sx_pipe_sequence
      sx_if_clause
        sx_compound_list
          sx_term \n
            sx_pipe_sequence
              sx_case_clause:
                tk_word: "abc"
                  (quoted) lx_normal: abc
                sx_compound_list:
                  tk_word: abc
                    lx_normal: abc
                  sx_term \n
                    sx_pipe_sequence
                      sx_simple_cmd
                        sx_wordlist
                          sx_word:
                            tk_word: echo
                              lx_normal: echo
                          sx_word:
                            tk_word: I
                              lx_normal: I
                          sx_word:
                            tk_word: kind
                              lx_normal: kind
                          sx_word:
                            tk_word: of
                              lx_normal: of
                          sx_word:
                            tk_word: wonder
                              lx_normal: wonder
                          sx_word:
                            tk_word: what
                              lx_normal: what
                          sx_word:
                            tk_word: will
                              lx_normal: will
                          sx_word:
                            tk_word: happen
                              lx_normal: happen
                          sx_word:
                            tk_word: here
                              lx_normal: here
                        sx_io_redirect_list
                          sx_lessand
                            sx_filename:
                              tk_word: -
                                lx_normal: -
                        sx_ass_list
                    sx_term \n
                      sx_pipe_sequence
                        sx_simple_cmd
                          sx_wordlist
                            sx_word:
                              tk_word: true
                                lx_normal: true
                          sx_io_redirect_list
                          sx_ass_list
                sx_compound_list:
                  tk_word: *
                    lx_normal: *
                  sx_term \n
                    sx_pipe_sequence
                      sx_simple_cmd
                        sx_wordlist
                          sx_word:
                            tk_word: false
                              lx_normal: false
                        sx_io_redirect_list
                        sx_ass_list
                sx_io_redirect_list
        sx_compound_list
          sx_term \n
            sx_pipe_sequence
              sx_simple_cmd
                sx_wordlist
                  sx_word:
                    tk_word: cat
                      lx_normal: cat
                sx_io_redirect_list
                sx_ass_list
            sx_term \n
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
                sx_simple_cmd
                  sx_wordlist
                    sx_word:
                      tk_word: ./fdcheck
                        lx_normal: ./fdcheck
                  sx_io_redirect_list
                  sx_ass_list
        sx_compound_list
          sx_term \n
            sx_pipe_sequence
              sx_simple_cmd
                sx_wordlist
                  sx_word:
                    tk_word: echo
                      lx_normal: echo
                  sx_word:
                    tk_word: WRONG
                      lx_normal: WRONG
                sx_io_redirect_list
                sx_ass_list
        sx_io_redirect_list
          sx_io_here
            sx_word:
              tk_word: TEST
                lx_normal: TEST
              HERE CONTENT:
                tk_word: I don't think there are use cases for this besides testing...

                  lx_normal: I don't think there are use cases for this besides testing...

0   minishell                           0x0000000101ec8c90 get_commandeer_proc + 320
1   minishell                           0x0000000101ec892f commandeer_inner + 383
2   minishell                           0x0000000101ef8ce2 commandeer_term + 194
3   minishell                           0x0000000101ec89bc commandeer_inner + 524
4   minishell                           0x0000000101ec9070 commandeer + 208
5   minishell                           0x0000000101eac1aa main + 810
6   libdyld.dylib                       0x00007fff5bffd3d5 start + 1
No command proc for 14
*/

int
	commandeer_term(const t_snode *node, const int io[SH_STDIO_SIZE])
{
	pid_t	pid;

	if (node->flags & flag_and)
	{
		pid = sh_fork();
		if (pid == 0)
		{
			ft_fprintf(sh()->io[SH_STDOUT_INDEX], "%s: %d\n", sh()->name, getpid());
			exit(commandeer_inner(node, io));
		}
		return (EXIT_SUCCESS);
	}
	return (commandeer_inner(node, io));
}
