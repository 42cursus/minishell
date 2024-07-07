/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 23:40:12 by yublee            #+#    #+#             */
/*   Updated: 2024/07/08 00:30:39 by yublee           ###   ########.fr       */
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

char	**get_argv(char *str, char **env, t_info info)
{
	char	**argv;
	int		i;
	char	*path;
	char	*tmp;
	char	**paths;

	i = 0;
	argv = ft_split_str_with_quotes(str, ' ');
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
