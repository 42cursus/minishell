/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 01:24:02 by abelov            #+#    #+#             */
/*   Updated: 2025/02/13 01:24:03 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * https://www.unix.com/programming/112624-readline-problems.html
 *
 * @return
 */
static int	ft_readline_event_hook(void)
{
	t_ctx *const	ctx = global_ctx(NULL, GET_VAL);

	if (g_received_signal_num == SIGINT)
	{
		g_received_signal_num = 0;
		if (ctx && ctx->prompt_type == PS_HERE)
		{
			ctx->received_signal_num = SIGINT;
			rl_replace_line("", 0);
			rl_done = 1;
		}
		else if (ctx && ctx->prompt_type == PS_REGULAR)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	return (0);
}

static void	ft_sh_parse_and_exec(t_ctx *ctx, const char *line, t_ast_node **ast)
{
	int	errcode;

	add_history(line);
	ft_memset(&ctx->hd, 0, sizeof(t_here_arr));
	ctx->hd.size = HEREDOC_ARRAY_SIZE;
	(*ast) = NULL;
	errcode = ft_do_parse(line, ast, ctx);
	free((char *)line);
	if ((*ast) != NULL && !errcode && ft_sh_collect_heredocs(ctx))
	{
		if (ft_sh_env_map_get_entry("DEBUG", ctx))
			print_ast((*ast), 0);
		ctx->last_status_code = ft_sh_execute_command((*ast), 0);
		free_ast((*ast));
	}
	unlink_herefiles(ctx);
}

int	ft_sh_loop(t_ctx *ctx)
{
	char			*line;
	t_ast_node		*ast;

	rl_event_hook = ft_readline_event_hook;
	ft_sh_init_welcome();
	ctx->argv = NULL;
	ctx->argc = 0;
	history_offset = history_length;
	while (ctx->last_status_code != EX_SHELL_EXIT)
	{
		line = ft_sh_read_line(ctx, PS_REGULAR);
		if (line)
		{
			if (*line != 0)
				ft_sh_parse_and_exec(ctx, line, &ast);
			else
				free(line);
		}
		else
			ctx->last_status_code = EX_SHELL_EXIT;
	}
	return (ctx->last_status_code);
}
