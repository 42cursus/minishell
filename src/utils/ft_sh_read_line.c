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
	t_sigaction	oldact;
	t_sigaction	*act;

	line = NULL;

	act = &ctx->act;
	if (type == PS_REGULAR)
		ft_sprintf(ps, ctx->ps1, count,
				   ft_sh_env_map_get_val("USER", ctx),
				   ft_sh_env_map_get_val("PWD", ctx));
	else
	{
		act = &ctx->hd_act;
		ft_sprintf(ps, ctx->ps2);
	}
	if (sigaction(SIGINT, act, &oldact))
		exit(((void) ft_sh_destroy_ctx(ctx), EXIT_FAILURE));
	line = readline(ps);
	if (sigaction(SIGINT, &oldact, NULL))
		exit(((void)ft_sh_destroy_ctx(ctx), EXIT_FAILURE));
	if (line && *line)
		count++;
	return (line);
}
