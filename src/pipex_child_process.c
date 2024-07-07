/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child_process.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/07/07 23:37:56 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	join_path(char **paths, char **args)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(temp);
		temp = paths[i];
		paths[i] = ft_strjoin(paths[i], *args);
		free(temp);
		i++;
	}
}

static void	check_path(char **paths, char **args)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		if (!access(paths[i], X_OK))
		{
			temp = *args;
			*args = ft_strdup(paths[i]);
			if (!*args)
				exit(EXIT_FAILURE);
			free(temp);
			break ;
		}
		i++;
	}
}

static char	**get_argv(char *str, char **env, t_info info)
{
	char	**argv;
	int		i;
	char	*path;
	char	*tmp;
	char	**paths;

	i = 0;
	argv = ft_split(str, ' ');
	if (!access(argv[0], X_OK))
		return (argv);
	while (!ft_strnstr(env[i], "PATH", 4))
		i++;
	path = ft_strnstr(env[i], "PATH", 4) + 5;
	paths = ft_split(path, ':');
	join_path(paths, &argv[0]);
	check_path(paths, &argv[0]);
	if (access(argv[0], X_OK))
	{
		tmp = ft_strdup(argv[0]);
		free_str_array(argv);
		free_str_array(paths);
		exit_with_message(tmp, 127, info);
	}
	free_str_array(paths);
	return (argv);
}

void	child_process(int i, t_list *current, t_info info)
{
	t_btree	*cmd;
	char	*cmd_str;
	char	**argv;

	cmd = (t_btree *)current->content;
	get_input(cmd, i, info);
	get_output(cmd, i, info);
	cmd_str = cmd->item;
	if (cmd_str[0] == 0)
		exit (EXIT_SUCCESS);
	argv = get_argv(cmd_str, info.env, info);
	if (execve(argv[0], argv, info.env) == -1)
	{
		free_str_array(argv);
		exit_with_message("execve", EXIT_FAILURE, info);
	}
}
