/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_lookup_pathname.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:35:31 by abelov            #+#    #+#             */
/*   Updated: 2025/01/22 00:35:31 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_sh_lookup_pathname(t_ctx *ctx)
{
	char	*str;
	char	*dup;
	char	*sptr;
	int 	not_found;
	char	*pathname;

	pathname = ctx->pathname;
	not_found = -1;
	pathname[0] = '\0';
	if (*(ctx->argv[0]) == '/' || *(ctx->argv[0]) == '.')
		not_found = access(ft_strncpy(pathname, ctx->argv[0], PATH_MAX), X_OK);
	else
	{
		dup = ft_strdup(ft_sh_env_map_get_val("PATH", ctx));
		if (dup)
		{
			str = ft_strtok_r(dup, ":", &sptr);
			while ((not_found == -1) && str)
			{
				ft_snprintf(pathname, PATH_MAX, "%s/%s", str, ctx->argv[0]);
				//TODO: early break
				not_found = access(pathname, X_OK);
				str = ft_strtok_r(NULL, ":", &sptr);
			}
			free(dup);
		}
	}
	return (not_found);
}
