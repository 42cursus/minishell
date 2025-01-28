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

char	**ft_get_argv(t_cmd_node *cmd, int *size, t_ctx *ctx)
{
	char	**argv;
	int		argc;
	t_wrd	*param;
	char	*word;

	argc = 0;
	param = cmd->args;
	while (param != NULL)
	{
		argc++;
		param = param->next_word;
	}
	argv = ft_calloc(argc + 1, sizeof(char *));

	if (ft_handle_err((t_error){ .assertion = (argv == NULL),
		.description = "Error allocating argv.",
		.func = __func__, .line = __LINE__, .file = __FILE__, .errnum = errno}))
		exit((ft_sh_destroy_ctx(ctx), SHELL_EXIT));
	param = cmd->args;
	argc = 0;
	while (param != NULL)
	{
		word = ft_get_word(param, ctx);
		if (ft_handle_err((t_error){ .assertion = (word == NULL),
			.description = "Error retrieving word.",
			.func = __func__, .line = __LINE__, .file = __FILE__, .errnum = errno}))
		{
			while (ctx->argc--)
				free(ctx->argv[ctx->argc]);
			exit((free(ctx->argv), ft_sh_destroy_ctx(ctx), SHELL_EXIT));
		}
		argv[argc++] = word;
		param = param->next_word;
	}
	*size = argc;
	return (argv);
}
