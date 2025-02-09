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

int ft_sh_lookup_pathname(t_ctx *ctx, t_cmd_node *cmd)
{
	char	*str;
	char	*sptr;
	char	*pathdup;
	char	*argv_zero;
	int 	errcode;

	errno = ENOENT;
	errcode = -1;
	argv_zero = ft_get_word(cmd->args, ctx);
	if (*argv_zero == '/' || *argv_zero == '.')
		errcode = access(ft_strncpy(ctx->pathname, argv_zero, PATH_MAX), F_OK);
	else if (ft_sh_env_map_get_val("PATH", ctx))
	{
		pathdup = ft_strdup(ft_sh_env_map_get_val("PATH", ctx));
		str = ft_strtok_r(pathdup, ":", &sptr);
		while ((errcode == -1 && errno == ENOENT) && str)
		{
			ft_snprintf(ctx->pathname, PATH_MAX, "%s/%s", str, argv_zero);
			errcode = access(ctx->pathname, F_OK);
			str = ft_strtok_r(NULL, ":", &sptr);
		}
		free(pathdup);
	}
	if ((errcode == -1 && errno == ENOENT))
		ft_perrorf("minishell: %s", argv_zero);
	free(argv_zero);
	return (errcode);
}
