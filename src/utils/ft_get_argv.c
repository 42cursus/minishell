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
	char **argv;
	int argc;

	t_wrd *param;
	argc = 1;

	param = cmd->args->next_word;
	while (param != NULL)
	{
		argc++;
		param = param->next_word;
	}

	argv = ft_calloc(argc + 1, sizeof(char *));
	if (argv == NULL)
	{
		ft_dprintf(STDERR_FILENO, "on %s at %s:%d", __FILE__, __LINE__, __func__);
		perror("Error allocating argv.");
		exit(1);
	}

	char *word;

	param = cmd->args;
	argc = 0;
	while (param != NULL)
	{
		word = ft_get_word(param, ctx);
		if (word == NULL)
		{
			ft_dprintf(STDERR_FILENO, "on %s at %s:%d", __FILE__, __LINE__, __func__);
			perror("Error retrieving word.");
			exit(1);
		}
		argv[argc++] = word;
		param = param->next_word;
	}
	*size = argc;
	return (argv);
}
