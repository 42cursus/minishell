/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chdir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 00:48:31 by abelov            #+#    #+#             */
/*   Updated: 2024/07/03 00:48:32 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_chdir_update_env_vars(t_ctx *ctx, char *oldpwd)
{
	ft_sh_env_map_bind_var(
		(t_sh_var){.k = ft_strdup("PWD"), .v = getcwd(NULL, 0)}, ctx);
	ft_sh_env_map_bind_var(
		(t_sh_var){.k = ft_strdup("OLDPWD"), .v = oldpwd}, ctx);
}

static int	ft_sh_cd_get_home(t_ctx *ctx, const char *path)
{
	char	*home;
	int		retval;

	retval = 0;
	home = ft_sh_env_map_get_val("HOME", ctx);
	if (home)
		ft_strncpy((char *) path, home, PATH_MAX);
	else
	{
		retval = -1;
		ft_dprintf(STDERR_FILENO, "minish: cd: HOME not set\n");
	}
	return (retval);
}

int	ft_chdir(t_ctx *ctx)
{
	char		path[PATH_MAX];
	char		*oldpwd;

	if (ctx->argc > 2)
	{
		errno = E2BIG;
		return (ft_perrorf("minishell: cd"), EXECUTION_FAILURE);
	}
	else if (ctx->argc == 2 && ft_strlen(ctx->argv[1]))
		ft_strncpy((char *) path, ctx->argv[1], PATH_MAX);
	else
	{
		if (ft_sh_cd_get_home(ctx, path))
			return (EXECUTION_FAILURE);
	}
	oldpwd = getcwd(NULL, 0);
	oldpwd = ft_sh_env_map_get_val("PWD", ctx);
	oldpwd = ".";
	if (chdir(path) != 0)
		return (free(oldpwd),
			ft_perrorf("minish: cd: %s", path), EXECUTION_FAILURE);
	ft_chdir_update_env_vars(ctx, oldpwd);
	return (0);
}
