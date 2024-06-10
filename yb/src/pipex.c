/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 22:28:10 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_info	get_info(t_list **cmd_list, t_btree *root, char **env)
{
	t_info	info;

	info.env = env;
	info.cmd_cnt = ft_lstsize(*cmd_list);
	info.cmd_list = cmd_list;
	info.root = root;
	return (info);
}

static int	**create_pipeline(int cnt)
{
	int	**fds;
	int	i;

	fds = (int **)malloc(cnt * sizeof(int *));
	if (!fds)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < cnt)
	{
		fds[i] = malloc(2 * sizeof(int));
		if (!fds[i])
		{
			free_fds(fds, i);
			exit(EXIT_FAILURE);
		}
		if (pipe(fds[i]) < 0)
		{
			free_fds(fds, i + 1);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (fds);
}

static void	child_process(int i, t_list *current, t_info info)
{
	t_btree	*cmd;
	char	*cmd_str;
	char	**args;

	cmd = (t_btree *)current->content;
	get_input(cmd, i, info);
	get_output(cmd, i, info);
	cmd_str = cmd->item;
	if (cmd_str[0] == 0)
		exit (EXIT_SUCCESS);
	args = get_args(cmd_str, info.env, info);
	if (execve(args[0], args, info.env) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE, info);
	}
}

void	exec_pipex(t_info info, t_list **cmd_list, int *status)
{
	int		i;
	pid_t	pid;
	t_list	*current;

	i = -1;
	current = *cmd_list;
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
	waitpid(pid, status, 0);
	free_before_exit(info);
	while (wait(NULL) != -1)
		;
}

void	pipex(t_list **cmd_list, t_btree *root, char **env)
{
	t_info	info;
	int		status;

	status = 0;
	info = get_info(cmd_list, root, env);
	info.fds = create_pipeline(info.cmd_cnt - 1);
	exec_pipex(info, cmd_list, &status);
	exit(WEXITSTATUS(status));
}
