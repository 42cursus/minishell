/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:59:19 by yublee            #+#    #+#             */
/*   Updated: 2024/07/07 23:56:53 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_before_exit(t_info	info)
{
	if (info.fds)
		free_fds(info.fds, info.cmd_cnt - 1);
	if (info.cmd_list)
		ft_lstclear(info.cmd_list, del);
	if (info.root)
		btree_apply_suffix(info.root, free_node);
}

void	exit_with_message(char *str, int exit_no, t_info info)
{
	if (str)
	{
		if (exit_no == 127)
		{
			write(2, str, ft_strlen(str));
			write(2, ": command not found\n", 20);
			free(str);
		}
		else
			perror(str);
	}
	free_before_exit(info);
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

int	free_fds(int **fds, int i)
{
	int	j;

	j = 0;
	while (j < i)
		free(fds[j++]);
	free(fds);
	return (1);
}
