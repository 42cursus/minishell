/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_finalise.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:24:46 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 14:24:47 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline.h>
#include "test.h"

/**
 * readline is messy and generates too many leaks
 * generating-suppressions-for-memory-leaks
 * https://stackoverflow.com/questions/17159578/
 */
int	ft_sh_destroy_ctx(t_ctx **ctx)
{
	int i;
	t_sh_var *var;

	i = -1;
	rl_clear_history();
	while (++i < (int)(*ctx)->env_map.total_elems)
	{
		var = &((t_sh_var *) (*ctx)->env_map.base)[i];
		free((void *) var->k);
		if (var->v != NULL)
			free(var->v);
	}
	free((*ctx)->env_map.base);
	while ((*ctx)->argc--)
		free((*ctx)->argv[(*ctx)->argc]);
	free((*ctx)->argv);
	free(*ctx);
	*ctx = NULL;
	return (0);
}
