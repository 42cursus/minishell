/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:16:31 by abelov            #+#    #+#             */
/*   Updated: 2025/02/13 01:16:32 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_builtin(t_cmd_node *cmd, t_ctx *const	ctx)
{
	int				retval;
	char			*inst;
	t_shell_op		*op;
	t_shell_op		*to_find;

	retval = 0;
	if (cmd && cmd->args)
	{
		inst = ft_get_word(cmd->args, ctx);
		to_find = &(t_shell_op){.inst = inst};
		op = ft_bsearch_obj(to_find, ctx->ops);
		if (op)
			retval = 1;
		free(inst);
	}
	return (retval);
}

int	ft_run_builtin(t_cmd_node *cmd, t_ctx *ctx)
{
	int				status;
	int				fd[3];
	t_shell_op		*op;

	fd[STDIN_FILENO] = dup(STDIN_FILENO);
	fd[STDOUT_FILENO] = dup(STDOUT_FILENO);
	fd[STDERR_FILENO] = dup(STDERR_FILENO);
	ctx->argv = ft_get_argv(cmd, &ctx->argc, ctx);
	op = ft_bsearch_obj((t_shell_op *)(&ctx->argv[0]), ctx->ops);
	status = ft_handle_redirects(cmd);
	if (!status)
		status = op->fun(ctx);
	dup2(fd[STDIN_FILENO], STDIN_FILENO);
	dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
	dup2(fd[STDERR_FILENO], STDERR_FILENO);
	close(fd[STDIN_FILENO]);
	close(fd[STDOUT_FILENO]);
	close(fd[STDERR_FILENO]);
	ft_cleanup_argv(ctx);
	return (status);
}
