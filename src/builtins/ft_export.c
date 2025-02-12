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
	char			*name;
	char			*sptr;
	t_sh_var *const	var = &(t_sh_var){.attrs = ATT_EXPORTED};
	int				i;
	int				retval;

	if (ctx->argc == 1)
		return (ft_env(ctx));
	i = 0;
	retval = EX_OK;
	while (++i < ctx->argc)
	{
		name = ft_strtok_r(ctx->argv[i], "=", &sptr);
		if (ft_sh_is_legal_identifier(name))
		{
			var->k = name;
			if (sptr != NULL)
				var->v = ft_strdup(sptr);
			ft_sh_env_map_bind_var(*var, ctx);
		}
		else
			retval = (ft_dprintf(STDERR_FILENO, "minishell: export: `%s':"
						" not a valid identifier\n", name), EXECUTION_FAILURE);
	}
	return (retval);
}
