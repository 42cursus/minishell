/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_env_map.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:48:06 by abelov            #+#    #+#             */
/*   Updated: 2024/07/05 12:48:07 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	ft_shell_parse_env_map(t_obj_arr *map, char **env_tab)
{
	size_t		i;
	size_t		map_size;
	t_sh_var	*env_map;
	char		*saveptr;
	char		*tmp;

	map_size = ft_get_tab_size((const void **) env_tab);
	env_map = (t_sh_var *)malloc(map_size * (sizeof(t_sh_var)));
	if (!env_map)
		return (-1);
	i = -1;
	while (++i < map_size)
	{
		saveptr = NULL;
		tmp = ft_strdup(env_tab[i]);
		env_map[i].k = ft_strtok_r(tmp, "=", &saveptr);
		env_map[i].attrs = ATT_EXPORTED | ATT_IMPORTED;
		if (saveptr != NULL)
			env_map[i].v = ft_strdup(saveptr);
	}
	map->base = env_map;
	map->elem_size = sizeof(t_sh_var);
	map->total_elems = map_size;
	map->cmp_fun = ft_shell_var_cmp;
	ft_qsort_obj(map);
	return (0);
}

t_sh_var	*ft_shell_env_map_get_entry(const char *key, t_exec_ctx *ctx)
{
	t_sh_var	*found;

	found = ft_bsearch_obj(&(t_sh_var){.k = key}, &ctx->env_map);
	return (found);
}

int	ft_shell_env_map_get_index(const char *key, t_exec_ctx *ctx)
{
	t_sh_var	*found;

	found = ft_bsearch_obj(&(t_sh_var){.k = key}, &ctx->env_map);
	return (-1);
}

int	ft_shell_env_map_add_entry(t_sh_var var, t_exec_ctx *ctx)
{
	t_obj_arr 	*obj;
	t_sh_var 	*new;
	size_t 		new_size;

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

int ft_shell_env_map_del_entry(t_sh_var var, t_exec_ctx *ctx)
{
	t_obj_arr 	*obj;
	t_sh_var 	*new;
	size_t 		new_size;

	obj = &ctx->env_map;
	new_size = obj->total_elems * obj->elem_size - obj->elem_size;
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

int	ft_shell_env_map_unbind_var(t_sh_var var, t_exec_ctx *ctx)
{
	if (ft_shell_env_map_get_entry(var.k, ctx))
		return (ft_shell_env_map_del_entry(var, ctx));
	return (0);
}



int	ft_shell_env_map_bind_var(t_sh_var var, t_exec_ctx *ctx)
{
	t_sh_var	*found;

	found = ft_shell_env_map_get_entry(var.k, ctx);
	if (found)
	{
		found->v = var.v;
		found->attrs = var.attrs;
		return (1);
	}
	return (ft_shell_env_map_add_entry(var, ctx));
}
