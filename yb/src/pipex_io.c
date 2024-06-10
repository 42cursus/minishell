/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 22:11:45 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	open_input(void *item, t_info info)
{
	int		fd_input;
	char	*str;

	str = (char *)item;
	fd_input = open(str + 1, O_RDONLY);
	if (fd_input < 0 || dup2(fd_input, STDIN_FILENO) < 0)
		exit_with_error(str + 1, EXIT_FAILURE, info);
	close(fd_input);
}
//heredoc needs to be added

static void	open_output(void *item, t_info info)
{
	int		fd_output;
	char	*str;

	str = (char *)item;
	if (*(str + 1) == '>')
		fd_output = open(str + 2, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		fd_output = open(str + 1, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd_output < 0)
		exit_with_error("output", EXIT_FAILURE, info);
	if (dup2(fd_output, STDOUT_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE, info);
	close(fd_output);
}
//heredoc needs to be added

void	get_input(t_btree *cmd, int i, t_info info)
{
	t_btree	*current;

	if (i != 0)
	{
		close(info.fds[i - 1][WRITE_END]);
		if (dup2(info.fds[i - 1][READ_END], STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i - 1][READ_END]);
	}
	current = cmd->left;
	while (current)
	{
		open_input(current->item, info);
		current = current->left;
	}
}

void	get_output(t_btree *cmd, int i, t_info info)
{
	t_btree	*current;

	if (i != info.cmd_cnt - 1)
	{
		close(info.fds[i][READ_END]);
		if (dup2(info.fds[i][WRITE_END], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i][WRITE_END]);
	}
	current = cmd->right;
	while (current)
	{
		open_output(current->item, info);
		current = current->right;
	}
}
