/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_env_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:48:06 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 14:21:56 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_sh_env_map_do_parse(char *const *env_tab, size_t map_size,
							t_sh_var *env_map)
{
	size_t	i;
	char	*saveptr;

	i = -1;
	while (++i < map_size)
	{
		saveptr = NULL;
		env_map[i].k = ft_strtok_r(ft_strdup(env_tab[i]), "=", &saveptr);
		env_map[i].attrs = ATT_EXPORTED | ATT_IMPORTED;
		if (saveptr != NULL)
			env_map[i].v = ft_strdup(saveptr);
	}
}

int	ft_sh_parse_env_map(t_obj_arr *map, char **env_tab)
{
	size_t		map_size;
	t_sh_var	*env_map;

	map_size = ft_tab_get_size((const void **) env_tab);
	env_map = (t_sh_var *)malloc(map_size * (sizeof(t_sh_var)));
	if (!env_map)
		return (-1);
	ft_sh_env_map_do_parse(env_tab, map_size, env_map);
	map->base = env_map;
	map->elem_size = sizeof(t_sh_var);
	map->total_elems = map_size;
	map->cmp_fun = ft_sh_var_cmp;
	ft_qsort_obj(map);
	return (0);
}

t_sh_var	*ft_sh_env_map_get_entry(const char *key, t_ctx *ctx)
{
	t_sh_var		*found;
	t_sh_var *const	to_find = &(t_sh_var){.k = key};

	found = ft_bsearch_obj(to_find, &ctx->env_map);
	return (found);
}

char	*ft_sh_env_map_get_val(const char *key, t_ctx *ctx)
{
	const char			*val;
	t_sh_var		*found;
	t_sh_var *const	to_find = &(t_sh_var){.k = key};

	val = NULL;
	found = ft_bsearch_obj(to_find, &ctx->env_map);
	if (found)
		val = found->v;
	return ((char *)val);
}

int	ft_sh_env_map_get_index(const char *key, t_ctx *ctx)
{
	int				idx;
	t_sh_var		*found;
	t_sh_var *const	to_find = &(t_sh_var){.k = key};

	idx = -1;
	found = ft_bsearch_obj(to_find, &ctx->env_map);
	if (found)
		idx = (int)(found - (t_sh_var *) ctx->env_map.base);
	return (idx);
}
