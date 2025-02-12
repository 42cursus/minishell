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

/**
 * below line 42 (2) is used for STDERR_FILENO and (1) for EXECUTION_FAILURE
 * 	due to norminette
 * @param ctx
 * @return
 */
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
			retval = (ft_dprintf(2, "%s: export: `%s': not a valid"
						" identifier\n", ctx->cmd_name, name), 1);
	}
	return (retval);
}
