/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 19:41:56 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_fds(int **fds, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(fds[j++]);
	free(fds);
	return (1);
}

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

void	pipex(t_list **cmd_list, t_btree *root, char **env)
{
	t_info	info;

	info = get_info(cmd_list, root, env);
	info.fds = create_pipeline(info.cmd_cnt - 1);
	exec_pipex(info, cmd_list);
}
