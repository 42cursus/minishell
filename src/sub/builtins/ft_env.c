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
	char *optr;
	char *sptr;

	optr = ft_strdup(ft_shell_env_map_get_entry("PATH", ctx)->v);
	str = ft_strtok_r(optr, ":", &sptr);
	while (str)
	{
		ft_putendl_fd(str, ctx->fdio.out);
		str = ft_strtok_r(NULL, ":", &sptr);
	}
	free(optr);
	return (0);
}
