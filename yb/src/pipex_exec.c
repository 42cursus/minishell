/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 18:35:15 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(char *cmd_str, t_info info)
{
	char	**args;

	if (cmd_str[0] == 0)
		exit (EXIT_SUCCESS);
	args = get_args(cmd_str, info.env, info);
	if (execve(args[0], args, info.env) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE, info);
	}
}

static void	get_input(t_btree *cmd, int i, t_info info)
{
	if (i != 0)
	{
		close(info.fds[i - 1][WRITE_END]);
		if (dup2(info.fds[i - 1][READ_END], STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i - 1][READ_END]);
	}
}

static void	get_output(t_btree *cmd, int i, t_info info)
{
	if (i != info.cmd_cnt - 1)
	{
		close(info.fds[i][READ_END]);
		if (dup2(info.fds[i][WRITE_END], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i][WRITE_END]);
	}
}

static void	child_process(int i, t_list *current, t_info info)
{
	t_btree	*cmd;
	
	cmd = (t_btree *)current;
	get_input(cmd, i, info);
	get_output(cmd, i, info);
	execute_cmd(cmd->item, info);
}

int	exec_pipex(t_info info, t_list *cmd_list)
{
	int		i;
	pid_t	pid;
	int		status;
	t_btree	*current;

	i = -1;
	status = 0;
	current = cmd_list;
	while (info.cmd_cnt && ++i < info.cmd_cnt)
	{
		pid = fork();
		if (pid < 0)
			exit_with_error("fork", EXIT_FAILURE, info);
		if (pid == 0)
			child_process(i, current, info);
		if (i != 0)
			close(info.fds[i - 1][READ_END]);
		if (i != info.cmd_cnt - 1)
			close(info.fds[i][WRITE_END]);
		current = current->next;
	}
	free_fds(info.fds, info.cmd_cnt - 1);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	exit(WEXITSTATUS(status));
}
