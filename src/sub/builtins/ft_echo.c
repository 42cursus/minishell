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

#include "test.h"

int	ft_echo(t_ctx *ctx)
{
	int i;

	i = 1;
	ft_putstr_fd(ctx->argv[i], ctx->fdio.out);
	while (++i < ctx->argc)
	{
		ft_putchar_fd(' ', ctx->fdio.out);
		ft_putstr_fd(ctx->argv[i], ctx->fdio.out);
	}
	ft_putchar_fd('\n', ctx->fdio.out);
	return (0);
}
