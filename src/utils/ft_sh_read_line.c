/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_read_line.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 22:16:02 by abelov            #+#    #+#             */
/*   Updated: 2024/07/23 22:16:03 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * https://stackoverflow.com/questions/38792542/readline-h-history-usage-in-c
 */
char	*ft_sh_read_line(t_ctx *ctx, t_prompt_type type)
{
	char		ps[MAXC];
	char		*line;
	static int	count = 1;

	line = NULL;
	if (type == PS_REGULAR)
	{
		ctx->prompt_type = PS_REGULAR;
		ft_sprintf(ps, ctx->ps1, ctx->last_status_code,
			ft_sh_env_map_get_val("USER", ctx),
			ft_sh_env_map_get_val("PWD", ctx));
	}
	else
	{
		ctx->prompt_type = PS_HERE;
		ft_sprintf(ps, ctx->ps2);
	}
	line = readline(ps);
	if (line && *line)
		count++;
	return (line);
}
