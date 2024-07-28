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

	i = 1;
	ft_putstr_fd(ctx->argv[i], STDOUT_FILENO);
	while (++i < ctx->argc)
	{
		ft_putchar_fd(' ', STDOUT_FILENO);
		ft_putstr_fd(ctx->argv[i], STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
