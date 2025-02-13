/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_run_forked.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:15:19 by abelov            #+#    #+#             */
/*   Updated: 2025/02/13 01:15:19 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "minishell.h"

int	ft_decode_wstatus(int wstatus)
{
	int	status;

	status = (EXIT_FAILURE);
	if (WIFSIGNALED (wstatus))
	{
		if (WCOREDUMP(wstatus))
			ft_dprintf (STDERR_FILENO, "Quit (core dumped)\n");
		else
			ft_dprintf (STDERR_FILENO, "Killed\n");
		status = (128 + WTERMSIG (wstatus));
	}
	else if (WIFSTOPPED(wstatus))
		status = (128 + WSTOPSIG(wstatus));
	else if (WIFEXITED(wstatus))
		status = (WEXITSTATUS (wstatus));
	return (status);
}

int	ft_wait_for_pid(int *wstatus, pid_t pid)
{
	waitpid(pid, wstatus, WUNTRACED);
	while (!WIFEXITED((*wstatus)) && !WIFSIGNALED((*wstatus)))
		waitpid(pid, wstatus, WUNTRACED);
	return (*wstatus);
}

int	ft_sh_wait_leader(const t_ast_node *cmd, pid_t fork_pid)
{
	int		wstatus;
	int		status;

	status = ft_decode_wstatus(ft_wait_for_pid(&wstatus, fork_pid));
	if (ft_getpid() != ft_tcgetpgrp(SHELL_TTY_FILENO))
		if (ft_sh_give_terminal_to(ft_getpid()))
			ft_perrorf("%s", cmd->ctx->cmd_name);
	return (status);
}

void	ft_sh_run_forked(t_ast_node *cmd, int level, const int *fd, int fd_idx)
{
	int	statuscode;

	ft_sh_sig_block(NULL);
	ft_reset_sighandlers(cmd->ctx);
	dup2(fd[fd_idx], fd_idx);
	close(fd[0]);
	close(fd[1]);
	statuscode = ft_sh_execute_command(cmd, level + 1);
	exit(statuscode);
}
