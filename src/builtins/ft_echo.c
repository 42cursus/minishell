/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 23:46:25 by abelov            #+#    #+#             */
/*   Updated: 2024/07/02 23:46:26 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_ctx *ctx)
{
	int	i;
	int	newline_opt;

	i = 1;
	newline_opt = 0;
	if (ctx->argv[i] && !ft_strncmp(ctx->argv[i], "-n", 2) && i++)
		newline_opt = 1;
	ft_printf("%s", ctx->argv[i]);
	while (++i < ctx->argc)
		ft_printf(" %s", ctx->argv[i]);
	if (!newline_opt)
		ft_printf("\n");
	return (0);
}
