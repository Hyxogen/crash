#include "commander.h"

#include <unistd.h>

int
	close_nostd_fd(int fd)
{
	if (fd != STDIN_FILENO && fd != STDOUT_FILENO && fd != STDERR_FILENO)
		return (sh_close(fd));
	return (0);
}

int
	command_setup_internal_redirects(const t_snode *redirect_list,
			const int io[SH_STDIO_SIZE], int old_io[SH_STDIO_SIZE])
{
	ft_memcpy(old_io, sh()->io, sizeof(sh()->io));
	ft_memcpy(sh()->io, io, sizeof(sh()->io));
	if (_cm_setup_builtin_redirects(redirect_list, sh()->io))
		return (-1);
	return (0);
}

void
	close_or_dup2_fd(int fromfd, int tofd)
{
	if (fromfd == SH_CLOSED_FD)
		sh_close(tofd);
	else
		sh_dup2(fromfd, tofd);
}

int
	command_setup_external_redirects(const t_snode *redirect_list,
			const int io[SH_STDIO_SIZE])
{
	sh_assert(redirect_list->type == sx_io_redirect_list);
	close_or_dup2_fd(io[SH_STDIN_INDEX], STDIN_FILENO);
	close_or_dup2_fd(io[SH_STDOUT_INDEX], STDOUT_FILENO);
	close_or_dup2_fd(io[SH_STDERR_INDEX], STDERR_FILENO);
	if (_cm_setup_process_redirects(redirect_list))
		return (-1);
	return (0);
}

int
	command_restore_internal_redirects(const int io[SH_STDIO_SIZE],
		const int old_io[SH_STDIO_SIZE])
{
	int	*shell_io;

	shell_io = sh()->io;
	if (shell_io[SH_STDIN_INDEX] != io[SH_STDIN_INDEX]
		&& shell_io[SH_STDIN_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDIN_INDEX]);
	if (shell_io[SH_STDOUT_INDEX] != io[SH_STDOUT_INDEX]
		&& shell_io[SH_STDOUT_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDOUT_INDEX]);
	if (shell_io[SH_STDERR_INDEX] != io[SH_STDERR_INDEX]
		&& shell_io[SH_STDERR_INDEX] != SH_CLOSED_FD)
		close_nostd_fd(shell_io[SH_STDERR_INDEX]);
	ft_memcpy(sh()->io, old_io, sizeof(sh()->io));
	return (0);
}
