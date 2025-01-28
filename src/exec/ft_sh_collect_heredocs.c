/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_collect_heredocs.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:09:03 by abelov            #+#    #+#             */
/*   Updated: 2025/01/27 16:09:04 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal_num = 0;

int	collect_heredocs_loop(t_ctx *ctx)
{
	int				fd;
	char			*line;
	t_hd_entry		*en;
	int				i;

	i = -1;
	while (++i < ctx->hd.size && g_received_signal_num != SIGINT)
	{
		en = &ctx->hd.entries[i];
		fd = open(en->filename, O_WRONLY | O_CREAT, DEFAULT_MODE);
		if (fd < 0)
			break ;
		g_received_signal_num = 0;
		line = ft_sh_read_line(ctx, PS_HERE);
		while (line && ft_strcmp(line, en->delimiter)
			&& g_received_signal_num != SIGINT)
		{
			add_history(line);
			ft_heredoc_file_lexing(fd, line, en->quotes, ctx);
			line = (free(line), ft_sh_read_line(ctx, PS_HERE));
		}
		(free(line), close(fd));
	}
	return (0);
}

int	ft_sh_collect_heredocs(t_ctx *ctx)
{
	if (!collect_heredocs_loop(ctx) && g_received_signal_num == SIGINT)
	{
		g_received_signal_num = 0;
		ctx->status_code = (-1);
		unlink_herefiles(ctx);
		return (0);
	}
	return (1);
}
