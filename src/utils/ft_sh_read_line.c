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
char	*ft_sh_read_line(t_ctx *ctx, const char *fmt)
{
	char 		ps[MAXC];
	char		*pwd;
	char		*p;
	char		*line;
	static int	count = 1;

	line = NULL;
	p = ft_sh_env_map_get_val("USER", ctx);
	pwd = ft_sh_env_map_get_val("PWD", ctx);

	if (fmt == NULL)
		fmt = "[%d] "FT_GREEN"%s"FT_RESET"@"FT_BLUE"%s"FT_RESET"-> ";
	sprintf(ps, fmt, count, p, pwd);
	line = readline(ps);
	if (line && *line)
		count++;
	return (line);
}

