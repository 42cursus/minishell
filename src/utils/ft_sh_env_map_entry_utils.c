/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_env_map_entry_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 15:59:10 by abelov            #+#    #+#             */
/*   Updated: 2024/07/22 15:59:12 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_sh_env_map_free_entry(t_sh_var *var)
{
	if (var)
		(free((void *)var->v), free((void *)var->k));
	return (0);
}

int	ft_sh_env_map_add_entry(t_sh_var var, t_ctx *ctx)
{
	t_obj_arr	*obj;
	t_sh_var	*new;

	obj = &ctx->env_map;
	new = ft_reallocarray(obj->base, obj->total_elems,
						  obj->total_elems  + 1, obj->elem_size);
	new[obj->total_elems] = var;
	obj->base = new;
	obj->total_elems++;
	ft_qsort_obj(obj);
	return (0);
}

int	ft_sh_env_map_del_entry(t_sh_var var, t_ctx *ctx)
{
	t_obj_arr	*obj;
	size_t		idx;

	obj = &ctx->env_map;
	idx = ft_sh_env_map_get_index(var.k, ctx);
	ft_sh_env_map_free_entry((t_sh_var *)obj->base + idx);
	ft_memmove(obj->base + (idx * obj->elem_size),
			   (obj->base + (idx + 1) * obj->elem_size),
			   (--(obj->total_elems) - idx) * obj->elem_size);
	return (0);
}
