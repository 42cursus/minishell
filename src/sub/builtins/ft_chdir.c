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

int	ft_chdir(t_exec_ctx *ctx)
{
	char 	*tmp;
	char	*path[PATH_MAX];

	tmp = ft_strdup(ctx->cmdline);
	ft_strtok(tmp, " ");
	ft_strncpy((char *) path, ft_strtok(NULL, " "), PATH_MAX);
	ft_putendl_fd(ft_strjoin("changing directory to: ", path),ctx->fdio.out);
	return (chdir(path));
}