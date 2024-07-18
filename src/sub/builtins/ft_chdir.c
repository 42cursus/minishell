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
	char	path[PATH_MAX];

	if (ctx->argv[1])
		ft_strncpy((char *) path, ctx->argv[1], PATH_MAX);
	else
		ft_strncpy((char *) path, ft_sh_env_map_get_entry("HOME", ctx)->v, PATH_MAX);
	ft_putendl_fd(ft_strjoin("changing directory to: ", path),ctx->fdio.out);
	return (chdir(path));
}
