/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 00:50:15 by abelov            #+#    #+#             */
/*   Updated: 2024/07/03 00:50:16 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline.h>
#include "minishell.h"

int	ft_exit(t_ctx *ctx)
{
	if (ctx->argc == 1)
	{
		ft_sh_destroy_ctx(ctx);
		exit(0);
	}
	return (-1);
}
