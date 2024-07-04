/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 18:34:59 by abelov            #+#    #+#             */
/*   Updated: 2024/06/28 18:34:59 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	ft_env(t_exec_ctx *ctx)
{
	char *str;

	str = ft_shell_env_map_get("PATH", ctx)->v;
	ft_putendl_fd(str, ctx->fdio.out);
	return (0);
}
