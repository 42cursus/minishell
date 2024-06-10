/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 19:36:26 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_error(char *str, int exit_no, t_info info)
{
	if (info.fds)
		free_fds(info.fds, info.cmd_cnt -1);
	if (exit_no == 127)
	{
		write(2, str, ft_strlen(str));
		write(2, ": command not found\n", 20);
		free(str);
	}
	else
		perror(str);
	exit(exit_no);
}

void	free_str_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

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
			free(temp);
			break ;
		}
		i++;
	}
}

char	**get_args(char *argv, char **env, t_info info)
{
	char	**args;
	int		i;
	char	*path;
	char	*tmp;
	char	**paths;

	i = 0;
	args = ft_split(argv, ' ');
	if (!access(args[0], X_OK))
		return (args);
	while (!ft_strnstr(env[i], "PATH", 4))
		i++;
	path = ft_strnstr(env[i], "PATH", 4) + 5;
	paths = ft_split(path, ':');
	join_path(paths, &args[0]);
	check_path(paths, &args[0]);
	if (access(args[0], X_OK))
	{
		tmp = ft_strdup(args[0]);
		free_str_array(args);
		free_str_array(paths);
		exit_with_error(tmp, 127, info);
	}
	free_str_array(paths);
	return (args);
}
