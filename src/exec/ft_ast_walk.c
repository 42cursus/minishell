/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ast_walk.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 01:55:16 by abelov            #+#    #+#             */
/*   Updated: 2025/01/05 01:55:16 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "minishell.h"

static int ft_run_on_pipe(t_ast_node *left, t_ast_node *right, int level,
						  t_ast_node *father)
{
	int fd[2];
	int status_cmd1;
	int status_cmd2;

	if (pipe(fd) < 0)
	{
		ft_dprintf(STDERR_FILENO, "on %s at %s:%d", __FILE__, __LINE__, __func__);
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
	pid_t pid_cmd1 = fork();
	if (pid_cmd1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		exit(ft_sh_execute(left, level + 1, father));
	}
	pid_t pid_cmd2 = fork();
	if (pid_cmd2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		exit(ft_sh_execute(right, level + 1, father));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_cmd1, &status_cmd1, 0);
	waitpid(pid_cmd2, &status_cmd2, 0);
	return status_cmd2;
}

static int ft_parse_simple(t_cmd_node *cmd)
{
	int			status;
	t_shell_op	*op;
	t_ctx		*ctx = cmd->ctx;
	int 		i;

	//TODO: do the redirects and return zero
	if (cmd == NULL || cmd->args == NULL || cmd->args->value == NULL)
	 	return (SHELL_EXIT);

	char cwd[1024];
	status = 0;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (SHELL_EXIT);

	if (!cmd->args) return (status);

	if (ft_strcmp(cmd->args->value, "exit") == 0 || ft_strcmp(cmd->args->value, "quit") == 0)
		return (SHELL_EXIT);

	ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);
	op = ft_bsearch_obj(&(t_shell_op) {.instruction = ctx->argv[0]}, ctx->ops);
	if (op != NULL)
		status = op->fun(ctx);
	else
		status = ft_sh_launch(cmd, ctx);
	while (ctx->argc--)
		free(ctx->argv[ctx->argc]);
	free(ctx->argv);
	ctx->argv = NULL;
	return (status);
}

int ft_sh_execute(t_ast_node *cmd, int level, t_ast_node *father)
{
	int exit_status;

	if (cmd == NULL)
		return (SHELL_EXIT);

	exit_status = SHELL_EXIT;
	if (cmd->type == NODE_COMMAND)
	{
		if (cmd->cmd != NULL)
		{
			cmd->cmd->ctx = cmd->ctx;
			exit_status = ft_parse_simple(cmd->cmd);
		}
	}
	else if (cmd->type == NODE_PIPE)
	{
		cmd->left->ctx = cmd->ctx;
		cmd->right->ctx = cmd->ctx;
		exit_status = ft_run_on_pipe(cmd->left, cmd->right, level, cmd);
	}
	return (exit_status);
	(void)father;
}

