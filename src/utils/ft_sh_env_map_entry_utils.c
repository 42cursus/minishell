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

int	ft_sh_env_map_free_entry(t_sh_var *var)
{
	if (var)
		(free(var->v), free((void *)var->k));
	return (0);
}

int	ft_sh_env_map_add_entry(t_sh_var var, t_ctx *ctx)
{
	t_obj_arr	*obj;
	t_sh_var	*new;
	size_t		new_size;

	obj = &ctx->env_map;
	new_size = obj->total_elems * obj->elem_size + obj->elem_size;
	new = (t_sh_var *) malloc(new_size);
	if (!new)
		return (-1);
	ft_memcpy(new, obj->base, obj->total_elems * obj->elem_size);
	new[obj->total_elems].k = var.k;
	new[obj->total_elems].v = var.v;
	new[obj->total_elems].attrs = var.attrs;
	free(obj->base);
	obj->base = new;
	obj->total_elems++;
	ft_qsort_obj(obj);
	return (0);
}

int	ft_sh_env_map_del_entry(t_sh_var var, t_ctx *ctx)
{
	t_obj_arr	*obj;
	t_sh_var	*new;
	size_t		new_size;
	size_t		idx;

	obj = &ctx->env_map;
	new_size = (obj->total_elems - 1) * obj->elem_size;
	idx = ft_sh_env_map_get_index(var.k, ctx);
	new = (t_sh_var *) malloc(new_size);
	if (!new)
		return (-1);
	ft_memcpy(new, obj->base, idx * obj->elem_size);
	ft_memcpy(new + idx, (t_sh_var *)obj->base + idx + 1,
		(obj->total_elems - idx) * obj->elem_size);
	ft_sh_env_map_free_entry((t_sh_var *)obj->base + idx);
	free(obj->base);
	obj->base = new;
	obj->total_elems--;
	return (0);
}
