/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 19:45:50 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(char *cmd_str, t_info info)
{
	char	**args;

	if (cmd_str[0] == 0)
		exit (EXIT_SUCCESS);
	args = get_args(cmd_str, info.env, info);
	if (execve(args[0], args, info.env) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE, info);
	}
}

static void	open_input(void *item, t_info info)
{
	int		fd_input;
	char	*str;

	str = (char *)item;
	fd_input = open(str + 1, O_RDONLY);
	if (fd_input < 0 || dup2(fd_input, STDIN_FILENO) < 0)
	{
		// ft_lstclear(info.cmd_list, del);
		// btree_apply_suffix(info.root, free_node);
		exit_with_error(str + 1, EXIT_FAILURE, info);
	}
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
	{
		// ft_lstclear(info.cmd_list, del);
		// btree_apply_suffix(info.root, free_node);
		exit_with_error("output", EXIT_FAILURE, info);
	}
	if (dup2(fd_output, STDOUT_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE, info);
	close(fd_output);
}
//heredoc needs to be added

static void	get_input(t_btree *cmd, int i, t_info info)
{
	if (i != 0)
	{
		close(info.fds[i - 1][WRITE_END]);
		if (dup2(info.fds[i - 1][READ_END], STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i - 1][READ_END]);
	}
	btree_apply_infix_only_left(cmd->left, info, open_input);
}

static void	get_output(t_btree *cmd, int i, t_info info)
{
	if (i != info.cmd_cnt - 1)
	{
		close(info.fds[i][READ_END]);
		if (dup2(info.fds[i][WRITE_END], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i][WRITE_END]);
	}
	btree_apply_infix_only_right(cmd->right, info, open_output);
}

static void	child_process(int i, t_list *current, t_info info)
{
	t_btree	*cmd;
	
	cmd = (t_btree *)current->content;
	get_input(cmd, i, info);
	get_output(cmd, i, info);
	execute_cmd(cmd->item, info);
}

int	exec_pipex(t_info info, t_list **cmd_list)
{
	int		i;
	pid_t	pid;
	int		status;
	t_list	*current;

	i = -1;
	status = 0;
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
	free_fds(info.fds, info.cmd_cnt - 1);
	waitpid(pid, &status, 0);
	ft_lstclear(cmd_list, del);
	btree_apply_suffix(info.root, free_node);
	while (wait(NULL) != -1)
		;
	exit(WEXITSTATUS(status));
}
