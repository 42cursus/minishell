/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 23:40:12 by yublee            #+#    #+#             */
/*   Updated: 2024/07/10 03:00:55 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	join_argv_to_valid_path(char **paths, char **argv)
{
	int		i;
	char	*temp;
	char	*new;

	i = 0;
	while (paths[i])
	{
		new = ft_strjoin(paths[i], "/");
		if (!new)
			exit(EXIT_FAILURE);
		temp = new;
		new = ft_strjoin(new, *argv);
		free(temp);
		if (!new)
			exit(EXIT_FAILURE);
		if (!access(new, X_OK))
		{
			free(*argv);
			*argv = new;
			return ;
		}
		free(new);
		i++;
	}
}

static char	*remove_paired_quotes(char *str)
{
	char	*new;
	char	quote;
	size_t	i;
	size_t	j;

	new = ft_strdup(str);
	if (!new)
		exit(EXIT_FAILURE);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (str[i] != quote)
				new[j++] = str[i++];
			i++;
		}
		else
			new[j++] = str[i++];
	}
	while (j < ft_strlen(str) + 1)
		new[j++] = 0;
	return (new);
}

static char	*find_type(char **argv, char **paths)
{
	static char	*builtins[5] = {"pwd", "env", "cd", "export", "unset"};
	int			i;
	char		*str;

	//needs to add echo later
	str = *argv;
	if (str[0] == '\"' || str[0] == '\'')
	{
		*argv = remove_paired_quotes(str);
		free(str);
		return ("str");
	}
	i = -1;
	while (++i < 5)
		if (!ft_strncmp(builtins[i], str, ft_strlen(str)))
			return ("builtin");
	if (str[0] != '/')
		join_argv_to_valid_path(paths, argv);
	if (!access(*argv, X_OK))
		return ("exec");
	if (!access(*argv, F_OK))
		return ("file");
	return ("NA");
}

char	**get_argv(char *str, char **env)
{
	char	**argv;
	char	**argv_type;
	char	*path;
	char	**paths;
	int		i;
	int		fd;// only for test

	argv = ft_split_str_with_quotes(str, ' ');
	i = 0;
	while (argv[i])
		i++;
	argv_type = (char **)malloc(sizeof(char *)*(i + 1));
	i = 0;
	while (!ft_strnstr(env[i], "PATH", 4))
		i++;
	path = ft_strnstr(env[i], "PATH", 4) + 5;
	paths = ft_split(path, ':');
	i = -1;
	while (argv[++i])
		argv_type[i] = find_type(&argv[i], paths);
	argv_type[i] = NULL;
	fd = open("args.txt", O_WRONLY | O_APPEND); //only for test
	for (int i = 0; argv[i]; i++) //only for test
		dprintf(fd, "arg[%d]:%s (%s)\n", i, argv[i], argv_type[i]); //only for test
	dprintf(fd, "----------------------------\n"); //only for test
	free_str_array(paths);
	free(argv_type);
	return (argv);
}
