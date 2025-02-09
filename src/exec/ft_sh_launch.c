/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_launch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:34:30 by abelov            #+#    #+#             */
/*   Updated: 2025/01/22 00:34:30 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "minishell.h"

static int exec_disc_command(t_cmd_node *cmd, t_ctx *ctx)
{
	int status;
	char **envp = ft_sh_render_envp(ctx);

	status = EXECUTION_SUCCESS;
	close(SHELL_TTY_FILENO);
	ft_reset_sighandlers(ctx);
	ft_handle_redirects(cmd);
	ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);
	status = execve(ctx->pathname, ctx->argv, envp);
	if (status)
	{
		if (errno == EACCES)
			status = EX_NOEXEC;
		ft_perrorf("minishell: %s", ctx->pathname);
	}
	ft_cleanup_envp(envp);
	ft_cleanup_argv(ctx);
	return (status);
}

void	ft_handle_redirects(t_cmd_node *cmd)
{
	if (cmd && cmd->redirects_in)
		ft_shell_handle_redirect(cmd->redirects_in, STDIN_FILENO, cmd->ctx, IN);
	if (cmd && cmd->redirects_out)
		ft_shell_handle_redirect(cmd->redirects_out, STDOUT_FILENO, cmd->ctx, OUT);
	if (cmd && cmd->redirects_err_in)
		ft_shell_handle_redirect(cmd->redirects_err_in, STDERR_FILENO, cmd->ctx, IN);
	if (cmd && cmd->redirects_err)
		ft_shell_handle_redirect(cmd->redirects_err, STDERR_FILENO, cmd->ctx, OUT);
}

int	ft_run_disc_command(t_cmd_node *cmd, t_ctx *ctx)
{
	pid_t		pid;
	int			status;
	int			wstatus;
	sigset_t	set;
	sigset_t	oldset;

	status = EX_NOTFOUND;
	if (cmd && !ft_sh_lookup_pathname(ctx, cmd))
	{
		if (ft_getpid() == ft_getpgrp())
		{
			pid = fork();
			if (pid == 0)
			{
				sigemptyset(&set);
				sigaddset(&set, SIGTTOU);
				sigaddset(&set, SIGTTIN);
				sigaddset(&set, SIGTSTP);
				sigaddset(&set, SIGSTOP);
				ft_sigprocmask(SIG_BLOCK, &set, &oldset);
				status = exec_disc_command(cmd, ctx);
				ft_sigprocmask(SIG_UNBLOCK, &oldset, NULL);
				ft_cleanup_argv(ctx);
				ft_sh_destroy_ctx(ctx);
				exit(status);
			}
			else if (pid < 0)
				status = (perror("minishell: error forking"), EX_MISCERROR);
			else
				status = (ft_decode_wstatus(ft_wait_for_pid(&wstatus, pid)));
		}
		else
			return (exec_disc_command(cmd, ctx));
	}
	return (status);
}


