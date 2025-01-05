/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 01:55:16 by abelov            #+#    #+#             */
/*   Updated: 2025/01/05 01:55:16 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "minishell.h"
#include "utils.h"

static int ft_run_on_pipe(t_ast_node *left, t_ast_node *right, int level,
						  t_ast_node *father)
{
	int fd[2];
	int status_cmd1;
	int status_cmd2;

	if (pipe(fd) < 0)
	{
		fprintf(stderr, "(%s:%d, %s): ", __FILE__, __LINE__, __func__);
		perror("pipe failed");
		exit(1);
	}

	pid_t pid_cmd1 = fork();

	if (pid_cmd1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);

		close(fd[0]);
		close(fd[1]);

		int status_code = traverse_and_exec_the_ast(left, level + 1, father);
		exit(status_code);
	}

	pid_t pid_cmd2 = fork();

	if (pid_cmd2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);

		close(fd[0]);
		close(fd[1]);


		int status_code = traverse_and_exec_the_ast(right, level + 1, father);
		exit(status_code);
	}

	close(fd[0]);
	close(fd[1]);

	waitpid(pid_cmd1, &status_cmd1, 0);
	waitpid(pid_cmd2, &status_cmd2, 0);

	return status_cmd2;
}

static int ft_parse_simple(t_cmd_node *cmd, int level, t_ast_node *father)
{
	int status;
	t_shell_op *op;
	t_ctx *ctx = cmd->ctx;

	// if (cmd == NULL || cmd->args == NULL || cmd->args->value == NULL)
	// 	return (SHELL_EXIT);

	char cwd[1024];
	status = 0;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (SHELL_EXIT);

	if (!cmd->args) return (status);

	if (strcmp(cmd->args->value, "exit") == 0 || strcmp(cmd->args->value, "quit") == 0)
		return (SHELL_EXIT);

	ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);

	op = ft_bsearch_obj(&(t_shell_op) {.instruction = ctx->argv[0]}, ctx->ops);
	if (op != NULL)
		status = op->fun(ctx);
	else
		status = ft_sh_launch(cmd, ctx);
//	while (--(ctx->argc))
//		free(ctx->argv[ctx->argc]);
	free(ctx->argv);
	ctx->argv = NULL;
	return (status);
}

int traverse_and_exec_the_ast(t_ast_node *cmd, int level, t_ast_node *father)
{
	int exit_status;

	if (cmd == NULL)
		return (SHELL_EXIT);

	if (cmd->type == NODE_COMMAND)
	{
		if (!cmd->cmd)
			return (SHELL_EXIT);
		cmd->cmd->ctx = cmd->ctx;
		return ft_parse_simple(cmd->cmd, level, father);
	}

	cmd->left->ctx = cmd->ctx;
	cmd->right->ctx = cmd->ctx;

	switch (cmd->type)
	{
		case NODE_PIPE:
			exit_status = ft_run_on_pipe(cmd->left, cmd->right, level, cmd);
			break;

		default:
			return (SHELL_EXIT);
	}

	return exit_status;
}


