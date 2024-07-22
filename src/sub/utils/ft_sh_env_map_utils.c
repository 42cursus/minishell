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

#include "test.h"

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
		found->v = var.v;
		found->attrs = var.attrs;
		return (1);
	}
	return (ft_sh_env_map_add_entry(var, ctx));
}

char	**ft_sh_render_envp(t_ctx *ctx)
{
	ssize_t		i;
	t_obj_arr	map;
	t_sh_var	*var;
	char		**new;

	map = ctx->env_map;
	new = (char **)malloc((map.total_elems + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	i = -1;
	while (++i < (int)map.total_elems)
	{
		var = map.base + (i * map.elem_size);
		new[i] = ft_strjoin2((const char *[]){var->k, var->v}, 2, "=");
	}
	new[i] = NULL;
	return (new);
}
