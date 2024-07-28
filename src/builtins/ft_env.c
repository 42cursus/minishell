/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 18:34:59 by abelov            #+#    #+#             */
/*   Updated: 2024/06/28 18:34:59 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_ctx *ctx)
{
	ssize_t	i;
	char	*str;
	char	**envp;

	i = 0;
	envp = ft_sh_render_envp(ctx);
	str = envp[i];
	while (str)
	{
		printf("%s\n", str);
		str = envp[++i];
	}
	ft_tab_str_free(envp);
	return (0);
}
