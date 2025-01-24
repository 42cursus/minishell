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

int	ft_sh_launch(t_cmd_node *cmd, t_ctx *ctx)
{
	pid_t	pid;
	int		status;

	status = -1;
	if (!ft_sh_lookup_pathname(ctx) && cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			ft_handle_redirects(cmd);
			if (execve(ctx->pathname, ctx->argv, ft_sh_render_envp(ctx)))
				return (perror("ft_sh: error in execve"), SHELL_EXIT);
		}
		else if (pid < 0)
			return (perror("ft_sh: error forking"), SHELL_EXIT);
		else
		{
			waitpid(pid, &status, WUNTRACED);
			while (!WIFEXITED(status) && !WIFSIGNALED(status))
				waitpid(pid, &status, WUNTRACED);
		}
	}
	else
		printf("%s: command not found\n", ctx->argv[0]);
	return (status);
}
