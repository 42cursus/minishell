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

int	exec_disc_command(t_cmd_node *cmd, t_ctx *ctx)
{
	int			status;
	char *const	*envp = ft_sh_render_envp(ctx);

	close(SHELL_TTY_FILENO);
	ft_reset_sighandlers(ctx);
	status = ft_handle_redirects(cmd);
	if (!status)
	{
		ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);
		status = execve(ctx->pathname, ctx->argv, envp);
		if (status)
		{
			if (errno == EACCES)
				status = EX_NOEXEC;
			ft_perrorf("minishell: %s", ctx->pathname);
		}
		ft_cleanup_argv(ctx);
	}
	ft_tab_str_free((void *)envp);
	return (status);
}

int	ft_handle_redirects(t_cmd_node *cmd)
{
	int	err_code;

	err_code = EX_OK;
	if (cmd && cmd->redirects_in)
		err_code = ft_shell_handle_redirect(cmd->redirects_in,
				STDIN_FILENO, cmd->ctx, IN);
	if (!err_code && cmd && cmd->redirects_out)
		err_code = ft_shell_handle_redirect(cmd->redirects_out,
				STDOUT_FILENO, cmd->ctx, OUT);
	if (!err_code && cmd && cmd->redirects_err_in)
		err_code = ft_shell_handle_redirect(cmd->redirects_err_in,
				STDERR_FILENO, cmd->ctx, IN);
	if (!err_code && cmd && cmd->redirects_err)
		err_code = ft_shell_handle_redirect(cmd->redirects_err,
				STDERR_FILENO, cmd->ctx, OUT);
	return (err_code);
}

void	ft_sh_sig_block(sigset_t *oldset)
{
	sigset_t	set;

	sigemptyset(&set);
	sigaddset(&set, SIGTTOU);
	sigaddset(&set, SIGTTIN);
	sigaddset(&set, SIGTSTP);
	sigaddset(&set, SIGSTOP);
	ft_sigprocmask(SIG_BLOCK, &set, oldset);
}

void	ft_exec_with_sig_block(t_cmd_node *cmd, t_ctx *ctx)
{
	int			status;
	sigset_t	oldset;

	ft_sh_sig_block(&oldset);
	status = exec_disc_command(cmd, ctx);
	ft_sigprocmask(SIG_UNBLOCK, &oldset, NULL);
	ft_cleanup_argv(ctx);
	ft_sh_destroy_ctx(ctx);
	exit(status);
}

int	ft_run_disc_command(t_cmd_node *cmd, t_ctx *ctx)
{
	pid_t		pid;
	int			status;
	int			wstatus;

	status = EX_NOTFOUND;
	if (cmd && !ft_sh_lookup_pathname(ctx, cmd))
	{
		if (ft_getpid() == ft_getpgrp())
		{
			pid = fork();
			if (pid == 0)
				ft_exec_with_sig_block(cmd, ctx);
			else if (pid < 0)
				status = (perror("minishell: error forking"), EX_SHELL_EXIT);
			else
				status = (ft_decode_wstatus(ft_wait_for_pid(&wstatus, pid)));
		}
		else
			return (exec_disc_command(cmd, ctx));
	}
	return (status);
}
