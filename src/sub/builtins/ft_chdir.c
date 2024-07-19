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

#include "test.h"

int	ft_chdir(t_ctx *ctx)
{
	char 		*home;
	char 		*oldpwd;
	char		path[PATH_MAX];
	char		cwd[PATH_MAX];

	cwd[0] = '\0';
	path[0] = '\0';
	oldpwd = ft_sh_env_map_get_entry("PWD", ctx)->v;
	if (!oldpwd)
	{
		oldpwd = ft_strdup("-");
		if (getcwd(cwd, PATH_MAX) != NULL)
			oldpwd = (free(oldpwd), ft_strdup(cwd));
	}
	if (ctx->argv[1])
		ft_strncpy((char *) path, ctx->argv[1], PATH_MAX);
	else
	{
		home = ft_sh_env_map_get_entry("HOME", ctx)->v;
		ft_strncpy((char *) path, home, PATH_MAX);
	}
	dprintf(ctx->fdio.out, "changing directory to: %s\n", path);
	if (chdir(path) != 0)
		return (free(oldpwd), -1);
	if (getcwd(cwd, PATH_MAX) != NULL)
		ft_sh_env_map_bind_var((t_sh_var){.k = "PWD", .v = ft_strdup(cwd)}, ctx);
	ft_sh_env_map_bind_var((t_sh_var){.k = "OLDPWD", .v = oldpwd}, ctx);
	return (0);
}
