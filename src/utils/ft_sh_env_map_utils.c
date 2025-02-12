/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_env_map_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 00:22:52 by abelov            #+#    #+#             */
/*   Updated: 2024/07/21 00:22:52 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_sh_env_map_unbind_var(t_sh_var var, t_ctx *ctx)
{
	t_sh_var	*to_find;

	to_find = ft_sh_env_map_get_entry(var.k, ctx);
	if (to_find)
		return (ft_sh_env_map_del_entry(var, ctx));
	return (0);
}

int	ft_sh_env_map_bind_var(t_sh_var var, t_ctx *ctx)
{
	t_sh_var	*found;

	found = ft_sh_env_map_get_entry(var.k, ctx);
	if (found)
	{
		free((void *)found->v);
		found->v = var.v;
		return (1);
	}
	return (ft_sh_env_map_add_entry(var, ctx));
}
