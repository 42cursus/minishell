/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 00:48:48 by abelov            #+#    #+#             */
/*   Updated: 2024/07/03 00:48:48 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_ctx *ctx)
{
	char		*str;
	char		*sptr;
	t_sh_var	var;

	if (ctx->argc == 1)
		return (ft_env(ctx));
	var = (t_sh_var){.k = NULL, .v = NULL};
	str = ft_strdup(*(ctx->argv));
	while (str && !ft_strchr(str, '='))
	{
		var.k = ft_strtok_r(str, "=", &sptr);
		if (sptr != NULL)
			var.v = ft_strdup(sptr);
		var.attrs = ATT_EXPORTED;
		str = *(ctx->argv);
		ft_sh_env_map_bind_var(var, ctx);
	}
	return (0);
}
