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
#include <fcntl.h>
#include "minishell.h"

int	ft_is_builtin(t_cmd_node *cmd, t_ctx *const	ctx)
{
	int				retval;
	char			*inst;
	t_shell_op		*op;
	t_shell_op		*to_find;

	retval = 0;
	if (cmd && cmd->args)
	{
		inst = ft_get_word(cmd->args, ctx);
		to_find = &(t_shell_op){.inst = inst};
		op = ft_bsearch_obj(to_find, ctx->ops);
		if (op)
			retval = 1;
		free(inst);
	}
	return (retval);
}

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

static int	ft_run_on_pipe(t_ast_node *left, t_ast_node *right, int level)
{
	int		fd[2];
	int		status;
	int		wstatus;
	pid_t	pid_cmd1;
	pid_t	pid_cmd2;

	wstatus = 0;
	if (pipe(fd) < 0)
		exit((ft_perrorf("minishell: pipe"), EXIT_FAILURE));
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

int	ft_run_builtin(t_cmd_node *cmd, t_ctx *ctx)
{
	int				status;
	int				fd[3];
	t_shell_op		*op;

	fd[STDIN_FILENO] = dup(STDIN_FILENO);
	fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
	fd[STDERR_FILENO] = dup(STDERR_FILENO);
	ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);
	op = ft_bsearch_obj((t_shell_op *)(&ctx->argv[0]), ctx->ops);
	status = ft_handle_redirects(cmd);
	if (!status)
		status = op->fun(ctx);
	dup2(fd[STDIN_FILENO], STDIN_FILENO);
	dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
	dup2(fd[STDERR_FILENO], STDERR_FILENO);
	close(fd[STDIN_FILENO]);
	close(fd[STDOUT_FILENO]);
	close(fd[STDERR_FILENO]);
	ft_cleanup_argv(ctx);
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

int	ft_sh_execute_command(t_ast_node *cmd, int level)
{
	int		status;
	int		wstatus;
	pid_t	fork_pid;
	pid_t	mypid;
	pid_t	original_pgrp;

	status = EX_MISCERROR;
	if (cmd && cmd->type == NODE_COMMAND && ft_is_builtin(cmd->cmd, cmd->ctx))
		status = ft_run_simple(cmd->cmd, cmd->ctx);
	else if (cmd && ((cmd->left && cmd->right) || cmd->cmd))
	{
		//TODO: int attr on SHLVL
		if (level == 0)
		{
			fork_pid = fork();
			if (fork_pid == 0)
			{
				mypid = ft_getpid();
				if (ft_setpgid(mypid, (pid_t)0) < 0)
					ft_dprintf(STDERR_FILENO,
						"leader setpgid (%d to %d)\n", mypid, 0);
				original_pgrp = ft_tcgetpgrp(SHELL_TTY_FILENO);
				if (ft_sh_give_terminal_to(mypid))
					ft_dprintf(STDERR_FILENO, "on %s at %s:%d\n",
						__func__, __FILE__, __LINE__);
				status = ft_run_other(cmd, level + 1);
				if (g_received_signal_num)
				{
					g_received_signal_num = 0;
					status = 128 + g_received_signal_num;
				}
				if (ft_sh_give_terminal_to(original_pgrp))
					ft_dprintf(STDERR_FILENO, "on %s at %s:%d\n",
						__func__, __FILE__, __LINE__);
				exit(status);
			}
			else if (fork_pid < 0)
				return (ft_perrorf("minishell: error"), EX_MISCERROR);
			else
			{
				status = ft_decode_wstatus(ft_wait_for_pid(&wstatus, fork_pid));
				if (ft_getpid() != ft_tcgetpgrp(SHELL_TTY_FILENO))
					if (ft_sh_give_terminal_to(ft_getpid()))
						ft_dprintf(STDERR_FILENO, "on %s at %s:%d\n",
							__func__, __FILE__, __LINE__);
			}
		}
		else
			status = ft_run_other(cmd, level + 1);
	}
	return (status);
}
