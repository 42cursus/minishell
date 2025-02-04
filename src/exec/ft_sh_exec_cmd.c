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

#include <sys/wait.h>
#include "minishell.h"

int	ft_is_builtin(t_cmd_node *cmd, t_ctx *const	ctx)
{
	int				retval;
	char			*inst;
	t_shell_op		*op;

	retval = 0;
	if (cmd && cmd->args)
	{
		inst = ft_get_word(cmd->args, ctx);
		op = ft_bsearch_obj(&(t_shell_op){ .inst = inst }, ctx->ops);
		if (op)
			retval = 1;
		free(inst);
	}
	return (retval);
}

static int ft_run_on_pipe(t_ast_node *left, t_ast_node *right, int level)
{
	int fd[2];
	int status_cmd1;
	int status_cmd2;

	if (pipe(fd) < 0)
	{
		ft_dprintf(STDERR_FILENO, "on %s at %s:%d", __FILE__, __LINE__, __func__);
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
	pid_t pid_cmd1 = fork();
	if (pid_cmd1 == 0)
	{
		(dup2(fd[1], STDOUT_FILENO),  close(fd[0]),  close(fd[1]));
		exit(ft_sh_execute_command(left, level + 1));
	}
	pid_t pid_cmd2 = fork();
	if (pid_cmd2 == 0)
	{
		(dup2(fd[0], STDIN_FILENO),  close(fd[0]),  close(fd[1]));
		exit(ft_sh_execute_command(right, level + 1));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_cmd1, &status_cmd1, 0);
	waitpid(pid_cmd2, &status_cmd2, 0);
	return status_cmd2;
}

static int ft_run_builtin(t_cmd_node *cmd, t_ctx *ctx)
{
	int				status;
	int 			fd[3];
	t_shell_op		*op;

	fd[STDIN_FILENO] = dup(STDIN_FILENO);
	fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
	fd[STDERR_FILENO] = dup(STDERR_FILENO);
	op = ft_bsearch_obj((t_shell_op *)(&ctx->argv[0]), ctx->ops);
	ft_handle_redirects(cmd);
	status = op->fun(ctx);
	dup2(fd[STDIN_FILENO], STDIN_FILENO);
	dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
	dup2(fd[STDERR_FILENO], STDERR_FILENO);
	close(fd[STDIN_FILENO]);
	close(fd[STDOUT_FILENO]);
	close(fd[STDERR_FILENO]);
	return (status);
}

static int ft_run_simple(t_cmd_node *cmd, t_ctx *ctx)
{
	int			status;

	status = 0;
	if (cmd->args)
	{
		ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);
		if (ft_is_builtin(cmd, ctx))
			status = ft_run_builtin(cmd, ctx);
		else
			status = ft_run_disc_command(cmd, ctx);
		while (ctx->argc--)
			free(ctx->argv[ctx->argc]);
		free(ctx->argv);
		ctx->argv = NULL;
	}
	return (status);
}

static int ft_run_other(t_ast_node *cmd, int level)
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

int ft_sh_execute_command(t_ast_node *cmd, int level)
{
	int		exit_status;
	int		wstatus;
	pid_t	fork_pid;
	pid_t	mypid;
	pid_t	original_pgrp;

	exit_status = SHELL_EXIT;
	if (cmd && cmd->type == NODE_COMMAND && ft_is_builtin(cmd->cmd, cmd->ctx))
		exit_status = ft_run_simple(cmd->cmd, cmd->ctx);
	else if (cmd && ((cmd->left && cmd->right) || cmd->cmd))
	{
		if (level == 0)
		{
			fork_pid = fork();
			if (fork_pid == 0)
			{
				mypid = ft_getpid();
				if (ft_setpgid(mypid, (pid_t)0) < 0)
				ft_dprintf(STDERR_FILENO, "leader setpgid (%d to %d)", mypid, 0);
				original_pgrp = ft_tcgetpgrp(SHELL_TTY_FILENO);
				ft_give_terminal_to(mypid);
				exit_status = ft_run_other(cmd, level);
				ft_give_terminal_to(original_pgrp);
				if (g_received_signal_num)
				{
					g_received_signal_num = 0;
					exit_status = 128 + SIGINT;
					signal(SIGINT, SIG_DFL);
					printf("\n");
					kill(ft_getpid(), SIGINT);
				}
				exit(exit_status);
			}
			else if (fork_pid < 0)
				return (perror("minishell: error forking"), SHELL_EXIT);
			else
			{
				waitpid(fork_pid, &wstatus, WUNTRACED);
				while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus))
					waitpid(fork_pid, &wstatus, WUNTRACED);
				if (ft_getpid() != ft_tcgetpgrp(SHELL_TTY_FILENO))
					ft_give_terminal_to(ft_getpid());
				if (WIFEXITED(wstatus))
					exit_status = WEXITSTATUS(wstatus);
			}
		}
		else
			exit_status = ft_run_other(cmd, level + 1);
	}
	return (exit_status);
}
