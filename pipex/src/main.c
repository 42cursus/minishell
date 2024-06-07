/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/10 12:27:29 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_info	get_info(int argc, char **argv, char **env)
{
	t_info	info;

	info.output = argv[argc - 1];
	info.env = env;
	info.here_doc = !ft_strncmp(argv[1], "here_doc", 8);
	info.cmd_cnt = argc - 3;
	if (info.here_doc == 0)
	{
		info.input = argv[1];
		info.h_end = NULL;
	}
	else
	{
		info.input = NULL;
		info.h_end = argv[2];
	}
	return (info);
}

int	free_fds(int **fds, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(fds[j++]);
	free(fds);
	return (1);
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

int	main(int argc, char *argv[], char **env)
{
	t_info	info;

	if (argc < 5)
	{
		write(2, "error: need more arguements\n", 28);
		exit(EXIT_FAILURE);
	}
	info = get_info(argc, argv, env);
	info.fds = create_pipeline(info.cmd_cnt - 1);
	pipex(info, argv);
}
