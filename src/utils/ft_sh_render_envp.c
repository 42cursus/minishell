/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_render_envp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:39:04 by abelov            #+#    #+#             */
/*   Updated: 2025/01/22 00:39:05 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		new[i] = ft_tab_str_join((const char *[2]){var->k, var->v}, 2, "=");
	}
	new[i] = NULL;
	return (new);
}
