/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:11:51 by yublee            #+#    #+#             */
/*   Updated: 2024/06/28 23:59:25 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_working_directory(t_ctx *ctx)
{
	const char	*cwd;
	const char	*getcwd_errstr = "getcwd: cannot access parent directories";

	cwd = ft_sh_env_map_get_val("PWD", ctx);
	if (!cwd)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			ft_perrorf("%s: error retrieving current directory: %s",
				ctx->cmd_name, getcwd_errstr);
			cwd = ft_strdup("");
		}
	}
	else
		cwd = ft_strdup(cwd);
	return ((char *)cwd);
}

int	ft_pwd(t_ctx *ctx)
{
	char	*cwd;
	int		retval;

	retval = EX_OK;
	cwd = get_working_directory(ctx);
	if (!errno)
		ft_dprintf(STDERR_FILENO, "%s\n", cwd);
	else
		retval = EXECUTION_FAILURE;
	free(cwd);
	return (retval);
}
