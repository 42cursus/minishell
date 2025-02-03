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

int	herefile_varname(int i, char *var, char *line)
{
	int		bi;
	char	c;

	bi = 0;
	while (line[++i] != '\0' && bi < ARG_MAX)
	{
		c = line[i];
		if (bi == 0 && (c == '?' || c == '$'))
		{
			var[bi++] = c;
			break ;
		}
		else if ((bi == 0 && (ft_isalpha(c) || c == '_'))
				 || ((bi > 0) && (ft_isalnum(c) || c == '_')))
			var[bi++] = c;
		else if (bi == 0 && !ft_isalpha(c))
			return (1);
		else
			break ;
	}
	var[bi] = '\0';
	if (bi > 0)
		return (0);
	return (1);
}

int	unlink_herefiles(t_ctx *ctx)
{
	const char	*filename;
	int			i;
	int			err;

	if (ctx && !ctx->hd.already_unlinked)
	{
		i = -1;
		while (++i < ctx->hd.size)
		{
			filename = ctx->hd.entries[i].filename;
			err = unlink(filename);
			if (err != 0)
				return (1);
		}
		ctx->hd.already_unlinked = 1;
		ctx->hd.ss = 0;
	}
	return (0);
}

void	herefile_expansion(int fd, const char *varname, t_ctx *ctx)
{
	const char	*value;

	if (ft_strcmp(varname, "$") == 0)
		ft_putnbr_fd(ft_getpid(), fd);
	else if (ft_strcmp(varname, "?") == 0)
		ft_putnbr_fd(ctx->status_code, fd);
	else
	{
		value = ft_sh_env_map_get_val(varname, ctx);
		if (value != NULL)
			ft_putstr_fd(value, fd);
	}
}

int	collect_heredocs_loop(t_ctx *ctx)
{
	int				fd;
	char			*line;
	t_hd_entry		*en;
	int				i;

	i = -1;
	while (++i < ctx->hd.size && ctx->g_received_signal_num != SIGINT)
	{
		en = &ctx->hd.entries[i];
		fd = open(en->filename, O_WRONLY | O_CREAT, DEFAULT_MODE);
		if (fd < 0)
			break ;
		ctx->g_received_signal_num = 0;
		line = ft_sh_read_line(ctx, PS_HERE);
		while (line && ft_strcmp(line, en->delimiter)
			&& ctx->g_received_signal_num != SIGINT)
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
	if (!collect_heredocs_loop(ctx) && ctx->g_received_signal_num == SIGINT)
	{
		ctx->g_received_signal_num = 0;
		ctx->status_code = (-1);
		unlink_herefiles(ctx);
		return (0);
	}
	return (1);
}
