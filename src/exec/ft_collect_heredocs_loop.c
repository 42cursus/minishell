/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_collect_heredocs_loop.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:32:47 by abelov            #+#    #+#             */
/*   Updated: 2025/02/12 15:32:47 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_collect_heredocs_loop(t_ctx *ctx)
{
	int				fd;
	char			*l;
	t_hd_entry		*en;
	int				i;

	i = -1;
	while (++i < ctx->hd.size && ctx->received_signal_num != SIGINT)
	{
		en = &ctx->hd.entries[i];
		fd = open(en->filename, O_WRONLY | O_CREAT, DEFAULT_MODE);
		if (fd < 0)
			return (1);
		ctx->received_signal_num = 0;
		l = ft_sh_read_line(ctx, PS_HERE);
		l = heredoc_line_loop(l, en, ctx, fd);
		if (l == NULL)
		{
			ft_dprintf(2, "%s: warning: here-document delimited "
				"by end-of-file (wanted '%s')\n", ctx->cmd_name, en->delimiter);
		}
		else
			free(l);
		close(fd);
	}
	return (0);
}
