/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_walk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 01:55:16 by abelov            #+#    #+#             */
/*   Updated: 2025/01/05 01:55:16 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_run_on_pipe(t_ast_node *left, t_ast_node *right, int level)
{
	int		fd[2];
	int		status;
	int		wstatus;
	pid_t	pid_cmd1;
	pid_t	pid_cmd2;

	wstatus = 0;
	if (pipe(fd) < 0)
		exit((ft_perrorf("%s: pipe", left->ctx->cmd_name), EXIT_FAILURE));
	pid_cmd1 = fork();
	if (pid_cmd1 == 0)
		ft_sh_run_forked(left, level, fd, STDOUT_FILENO);
	pid_cmd2 = fork();
	if (pid_cmd2 == 0)
		ft_sh_run_forked(right, level, fd, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_wait_for_pid(&wstatus, pid_cmd1);
	status = ft_decode_wstatus(ft_wait_for_pid(&wstatus, pid_cmd2));
	return (status);
}

static int	ft_run_simple(t_cmd_node *cmd, t_ctx *ctx)
{
	int			status;

	status = 0;
	if (cmd->args)
	{
		if (ft_is_builtin(cmd, ctx))
			status = ft_run_builtin(cmd, ctx);
		else
			status = ft_run_disc_command(cmd, ctx);
	}
	return (status);
}

static int	ft_run_other(t_ast_node *cmd, int level)
{
	int	exit_status;

	if (cmd->type == NODE_COMMAND && cmd->cmd)
		exit_status = ft_run_simple(cmd->cmd, cmd->ctx);
	else
	{
		cmd->left->ctx = cmd->ctx;
		cmd->right->ctx = cmd->ctx;
		if (cmd->type == NODE_AND)
		{
			exit_status = ft_sh_execute_command(cmd->left, level + 1);
			if (!exit_status)
				exit_status = ft_sh_execute_command(cmd->right, level + 1);
		}
		if (cmd->type == NODE_OR)
		{
			exit_status = ft_sh_execute_command(cmd->left, level + 1);
			if (exit_status)
				exit_status = ft_sh_execute_command(cmd->right, level + 1);
		}
		else if (cmd->type == NODE_PIPE)
			exit_status = ft_run_on_pipe(cmd->left, cmd->right, level);
	}
	return (exit_status);
}

void	ft_sh_run_leader(t_ast_node *cmd, int level, int status)
{
	pid_t	mypid;
	pid_t	original_pgrp;

	mypid = ft_getpid();
	if (ft_setpgid(mypid, (pid_t) 0) < 0)
		ft_dprintf(STDERR_FILENO,
			"leader setpgid (%d to %d)\n", mypid, 0);
	original_pgrp = ft_tcgetpgrp(SHELL_TTY_FILENO);
	if (ft_sh_give_terminal_to(mypid))
		ft_perrorf("%s", cmd->ctx->cmd_name);
	status = ft_run_other(cmd, level + 1);
	if (g_received_signal_num)
	{
		status = 128 + g_received_signal_num;
		g_received_signal_num = 0;
	}
	if (ft_sh_give_terminal_to(original_pgrp))
		ft_perrorf("%s", cmd->ctx->cmd_name);
	ft_sh_destroy_ctx(cmd->ctx);
	free_ast(cmd);
	rl_clear_history();
	exit(status);
}

int	ft_sh_execute_command(t_ast_node *cmd, int level)
{
	int		status;
	pid_t	fork_pid;

	status = EX_MISCERROR;
	if (cmd && cmd->type == NODE_COMMAND && ft_is_builtin(cmd->cmd, cmd->ctx))
		status = ft_run_simple(cmd->cmd, cmd->ctx);
	else if (cmd && ((cmd->left && cmd->right) || cmd->cmd))
	{
		if (level != 0)
			status = ft_run_other(cmd, level + 1);
		else
		{
			fork_pid = fork();
			if (fork_pid == 0)
				ft_sh_run_leader(cmd, level, status);
			else if (fork_pid < 0)
				return (ft_perrorf("%s: error",
						cmd->ctx->cmd_name), EX_MISCERROR);
			else
				status = ft_sh_wait_leader(cmd, fork_pid);
		}
	}
	return (status);
}
