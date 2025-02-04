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

	status = 0;
	close(SHELL_TTY_FILENO);
	ft_reset_sighandlers(ctx);
	ft_handle_redirects(cmd);
	ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);
	if (execve(ctx->pathname, ctx->argv, ft_sh_render_envp(ctx)))
	{
		perror("ft_sh: error in execve");
		if (errno == EACCES)
			status = 129;
		ft_sh_destroy_ctx(ctx);
	}
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
	pid_t	pid;
	int		status;
	int		wstatus;

	status = 127;
	if (cmd && !ft_sh_lookup_pathname(ctx, cmd))
	{
		pid_t myppid = ft_getpid();
		pid_t pgrp = ft_getpgrp();
		if (myppid != pgrp)
			exit(exec_disc_command(cmd, ctx));
		else
		{
			pid = fork();
			if (pid == 0)
				exit(exec_disc_command(cmd, ctx));
			else if (pid < 0)
				return (perror("ft_sh: error forking"), SHELL_EXIT);
			else
			{
				waitpid(pid, &wstatus, WUNTRACED);
				while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus))
					waitpid(pid, &wstatus, WUNTRACED);
				if (WIFEXITED(wstatus))
					status = WEXITSTATUS(wstatus);
			}
		}
	}
	else
		ft_dprintf(STDERR_FILENO, "%s: command not found\n", ctx->argv[0]);
	return (status);
}


