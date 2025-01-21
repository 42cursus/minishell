/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 02:30:50 by abelov            #+#    #+#             */
/*   Updated: 2025/01/05 02:30:51 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

#include "utils.h"
#include "parser.h"
#include "minishell.h"

void parse_error(const char *str, const int where)
{
	ft_dprintf(STDERR_FILENO, "Parse error near %d: %s\n", where, str);
}

static int parse_simple(simple_cmd_t *s, int level, cmd_t *father)
{
	int status;
	t_shell_op *op;
	t_ctx *ctx = s->ctx;

	if (s == NULL || s->verb == NULL || s->verb->string == NULL)
		return (SHELL_EXIT);

	if (!s->verb->expand && s->verb->next_part && !ft_strcmp(s->verb->next_part->string, "="))
	{
		const char *key, *val = NULL;
		key = s->verb->string;
		if (s->verb->next_part->next_part)
			val = get_word(s->verb->next_part->next_part, s->ctx);
		setenv(key, val, 1);
		ft_sh_env_map_bind_var((t_sh_var) {.k = ft_strdup(key), .v = ft_strdup(val)}, ctx);
		word_t *param = s->params;
		word_t *curr;
		free((void *)val);
		while (param && !param->expand && param->next_part && !ft_strcmp(param->next_part->string, "="))
		{
			val = NULL;
			key = param->string;
			if (param->next_part->next_part)
				val = get_word(param->next_part->next_part, s->ctx);
			ft_sh_env_map_bind_var((t_sh_var) {.k = ft_strdup(key), .v = ft_strdup(val)}, ctx);
			curr = param;
			param = param->next_word;
			free(curr);
			free((void *)val);
		}
		if (!param)
			return true;
		s->params = param->next_word;
		param->next_word = NULL;
	}

	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (SHELL_EXIT);
	if (strcmp(s->verb->string, "exit") == 0 || strcmp(s->verb->string, "quit") == 0)
		return (SHELL_EXIT);
	ctx->argv = get_argv(s, &ctx->argc, ctx);
	op = ft_bsearch_obj(&(t_shell_op) {.instruction = ctx->argv[0]}, ctx->ops);
	if (op != NULL)
		status = op->fun(ctx);
	else
		status = ft_sh_launch2(ctx, s);
	while (ctx->argc-- > 0)
		free(ctx->argv[ctx->argc]);
	free(ctx->argv);
	ctx->argv = NULL;
	return (status);
}

static bool run_in_parallel(cmd_t *cmd1, cmd_t *cmd2, int level, cmd_t *father)
{
	pid_t	pid_cmd1;
	pid_t	pid_cmd2;
	int		status_cmd1;
	int		status_cmd2;

	pid_cmd1 = fork();
	if (pid_cmd1 == 0)
		exit(traverse_and_exec_the_ast2(cmd1, level + 1, father));
	pid_cmd2 = fork();
	if (pid_cmd2 == 0)
		exit(traverse_and_exec_the_ast2(cmd2, level + 1, father));
	waitpid(pid_cmd1, &status_cmd1, 0);
	waitpid(pid_cmd2, &status_cmd2, 0);
	return status_cmd1 == 0 && status_cmd2 == 0;
}

static bool run_on_pipe(cmd_t *left, cmd_t *right, int level, cmd_t *father)
{
	int fd[2];
	int status_cmd1;
	int status_cmd2;

	if (pipe(fd) < 0)
	{
		ft_dprintf(STDERR_FILENO, "on %s at %s:%d", __FILE__, __LINE__, __func__);
		perror("pipe failed");
		ft_exit(EXIT_FAILURE);
	}
	pid_t pid_cmd1 = fork();
	if (pid_cmd1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		exit(traverse_and_exec_the_ast2(left, level + 1, father));
	}
	pid_t pid_cmd2 = fork();
	if (pid_cmd2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		exit(traverse_and_exec_the_ast2(right, level + 1, father));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_cmd1, &status_cmd1, 0);
	waitpid(pid_cmd2, &status_cmd2, 0);
	return (status_cmd2);
}

int traverse_and_exec_the_ast2(cmd_t *c, int level, cmd_t *father)
{
	int exit_status;

	if (c == NULL)
		return (SHELL_EXIT);

	if (c->op == OP_SIMPLE)
	{
		if (!c->simple_cmd)
			return (SHELL_EXIT);
		c->simple_cmd->ctx = c->ctx;
		return parse_simple(c->simple_cmd, level, father);
	}

	c->left->ctx = c->ctx;
	c->right->ctx = c->ctx;

	switch (c->op)
	{
		case OP_SEQUENTIAL:
			exit_status = traverse_and_exec_the_ast2(c->left, level + 1, c);
			if (exit_status >= 0)
				exit_status = traverse_and_exec_the_ast2(c->right, level + 1, c);
			break;

		case OP_PARALLEL:
			exit_status = run_in_parallel(c->left, c->right, level, c);
			break;

		case OP_CONDITIONAL_NZERO:
			exit_status = traverse_and_exec_the_ast2(c->left, level + 1, c);
			exit_status = exit_status == 0 ? traverse_and_exec_the_ast2(c->right, level + 1, c) : exit_status;
			break;

		case OP_CONDITIONAL_ZERO:
			exit_status = traverse_and_exec_the_ast2(c->left, level + 1, c);
			exit_status = exit_status != 0 ? traverse_and_exec_the_ast2(c->right, level + 1, c) : exit_status;
			break;

		case OP_PIPE:
			exit_status = run_on_pipe(c->left, c->right, level, c);
			break;

		default:
			return (SHELL_EXIT);
	}
	return exit_status;
}
