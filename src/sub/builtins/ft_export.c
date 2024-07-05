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

#include "test.h"

int	ft_export(t_exec_ctx *ctx)
{
	char *str;
	char *optr;
	char *sptr1;
	char *saveptr;
	t_sh_var var;

	optr = ft_strtok_r(ft_strdup(ctx->cmdline), " ", &sptr1);
	str = ft_strtok_r(NULL, " ", &sptr1);;
	while (str)
	{
		saveptr = NULL;
		var.k = ft_strdup(ft_strtok_r(str, "=", &saveptr));
		var.attrs = ATT_EXPORTED;
		var.v = NULL;
		if (saveptr != NULL)
			var.v = ft_strdup(saveptr);
		ft_shell_env_map_bind_var(var, ctx);
		str = ft_strtok_r(NULL, " ", &sptr1);
		ft_putendl_fd(ft_shell_env_map_get_entry(var.k, ctx)->v, ctx->fdio.out);
	}
	free(optr);
	return (0);
}
