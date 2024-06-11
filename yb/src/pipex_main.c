/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/06/11 15:46:31 by yublee           ###   ########.fr       */
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

static void	wait_if_heredoc(void *content)
{
	t_btree	*tmp;
	char	*str;

	tmp = (t_btree *)content;
	tmp = tmp->left;
	while (tmp)
	{
		str = (char *)tmp->item;
		if (str && str[0] == '<' && str[1] == '<')
		{
			wait(NULL);
			break ;
		}
		tmp = tmp->left;
	}
}

static void	exec_pipex(t_info info, t_list **cmd_list, int *status)
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
			exit_with_message("fork", EXIT_FAILURE, info);
		if (pid == 0)
			child_process(i, current, info);
		if (i != 0)
			close(info.fds[i - 1][READ_END]);
		if (i != info.cmd_cnt - 1)
			close(info.fds[i][WRITE_END]);
		wait_if_heredoc(current->content);
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
