/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:37:53 by yublee            #+#    #+#             */
/*   Updated: 2024/05/10 12:27:46 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	execute_cmd(char *cmd, t_info info)
{
	char	**args;

	args = get_args(cmd, info.env, info);
	if (execve(args[0], args, info.env) == -1)
	{
		free_str_array(args);
		exit_with_error("execve", EXIT_FAILURE, info);
	}
}

static void	get_output(t_info info)
{
	int	fd_output;

	if (!info.here_doc)
		fd_output = open(info.output, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	else
		fd_output = open(info.output, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (fd_output < 0)
		exit_with_error("output", EXIT_FAILURE, info);
	if (dup2(fd_output, STDOUT_FILENO) < 0)
		exit_with_error("dup2", EXIT_FAILURE, info);
	close(fd_output);
}

static void	get_input(t_info info)
{
	int		fd_input;
	char	*buf;

	if (info.here_doc)
	{
		close(info.fds[0][READ_END]);
		while (1)
		{
			buf = get_next_line(0);
			if (!ft_strncmp(buf, info.h_end, ft_strlen(info.h_end)))
				break ;
			write(info.fds[0][WRITE_END], buf, ft_strlen(buf));
			free(buf);
		}
		free(buf);
		close(info.fds[0][WRITE_END]);
		close(0);
		buf = get_next_line(0);
		free_fds(info.fds, info.cmd_cnt - 1);
		exit(EXIT_SUCCESS);
	}
	fd_input = open(info.input, O_RDONLY);
	if (fd_input < 0 || dup2(fd_input, STDIN_FILENO) < 0)
		exit_with_error("input", EXIT_FAILURE, info);
	close(fd_input);
}

static void	child_process(int i, char *cmd, t_info info)
{
	if (i != 0)
	{
		close(info.fds[i - 1][WRITE_END]);
		if (dup2(info.fds[i - 1][READ_END], STDIN_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i - 1][READ_END]);
	}
	else
		get_input(info);
	if (i != info.cmd_cnt - 1)
	{
		close(info.fds[i][READ_END]);
		if (dup2(info.fds[i][WRITE_END], STDOUT_FILENO) < 0)
			exit_with_error("dup2", EXIT_FAILURE, info);
		close(info.fds[i][WRITE_END]);
	}
	else
		get_output(info);
	execute_cmd(cmd, info);
}

int	pipex(t_info info, char **argv)
{
	int		i;
	pid_t	pid;
	int		status;

	i = -1;
	status = 0;
	while (info.cmd_cnt && ++i < info.cmd_cnt)
	{
		pid = fork();
		if (pid < 0)
			exit_with_error("fork", EXIT_FAILURE, info);
		if (pid == 0)
			child_process(i, argv[i + 2], info);
		if (i == 0 && info.here_doc)
			wait(NULL);
		if (i != 0)
			close(info.fds[i - 1][READ_END]);
		if (i != info.cmd_cnt - 1)
			close(info.fds[i][WRITE_END]);
	}
	free_fds(info.fds, info.cmd_cnt - 1);
	waitpid(pid, &status, 0);
	while (wait(NULL) != -1)
		;
	exit(WEXITSTATUS(status));
}
