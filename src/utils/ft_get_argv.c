/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:30:05 by abelov            #+#    #+#             */
/*   Updated: 2025/01/22 00:30:06 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_sh_psize(t_wrd	*param)
{
	int	size;

	if (!param)
		return (0);
	size = 1;
	while (param->next_word)
	{
		param = param->next_word;
		size++;
	}
	return (size);
}

char	**ft_get_argv(t_cmd_node *cmd, int *size, t_ctx *ctx)
{
	char	**argv;
	int		argc;
	t_wrd	*param;
	char	*word;

	argv = ft_calloc(ft_sh_psize(cmd->args) + 1, sizeof(char *));
	if (!argv)
	{
		ft_perrorf("%s: Error allocating argv.", ctx->cmd_name);
		exit((ft_sh_destroy_ctx(ctx), EX_SHELL_EXIT));
	}
	argc = 0;
	param = cmd->args;
	while (param != NULL)
	{
		word = ft_get_word(param, ctx);
		if (!word)
			exit((ft_sh_destroy_ctx(ctx), EX_SHELL_EXIT));
		argv[argc++] = word;
		param = param->next_word;
	}
	*size = argc;
	return (argv);
}
