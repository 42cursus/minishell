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
	char 		*oldpwd;
	char 		*pwd;
	char		path[PATH_MAX];
	char		cwd[PATH_MAX];
	t_sh_var 	*var;

	cwd[0] = '\0';
	path[0] = '\0';

	oldpwd = ft_strdup(ft_sh_env_map_get_val("PWD", ctx));
	if (getcwd(cwd, PATH_MAX) != NULL)
		oldpwd = (free(oldpwd), ft_strdup(cwd));
	if (ctx->argv[1])
		ft_strncpy((char *) path, ctx->argv[1], PATH_MAX);
	else
	{
		pwd = ft_sh_env_map_get_val("HOME", ctx);
		if (pwd != NULL)
			ft_strncpy((char *) path, pwd, PATH_MAX);
		else
		{
			free(oldpwd);
			dprintf(ctx->fdio.out, "minsh: cd: HOME not set");
			return (-1);
		}
	}
	dprintf(ctx->fdio.out, "changing directory to: %s\n", path);
	if (chdir(path) != 0)
		return (free(oldpwd), -1);
	pwd = ft_strdup(path);
	if (getcwd(cwd, PATH_MAX) != NULL)
		pwd = (free(pwd), ft_strdup(cwd));
	var = ft_sh_env_map_get_entry("PWD", ctx);
	if (var)
		var->v = (free(var->v), pwd);
	else
		ft_sh_env_map_bind_var((t_sh_var){.k = ft_strdup("PWD"), .v = pwd}, ctx);
	var = NULL;
	var = ft_sh_env_map_get_entry("OLDPWD", ctx);
	if (var)
		var->v = (free(var->v), oldpwd);
	else
		ft_sh_env_map_bind_var((t_sh_var){.k = ft_strdup("OLDPWD"), .v = oldpwd}, ctx);
	return (0);
}
