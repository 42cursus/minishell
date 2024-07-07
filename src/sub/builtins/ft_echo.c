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

int	ft_echo(t_exec_ctx *ctx)
{
	int argc;

	argc = ctx->argc;
	while (argc--)
		ft_putendl_fd(ctx->argv[ctx->argc], ctx->fdio.out);
	return (0);
}
