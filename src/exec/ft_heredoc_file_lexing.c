/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_file_lexing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:12:26 by abelov            #+#    #+#             */
/*   Updated: 2025/01/27 16:12:27 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * What is the maximum size of a Linux environment variable value?
 * 	https://stackoverflow.com/questions/1078031/1078125#1078125
 * 	https://man7.org/linux/man-pages/man2/execve.2.html
 */
void	ft_heredoc_file_lexing(int fd, char *line, bool quotes, t_ctx *ctx)
{
	int		i;
	int		e;
	char	var[ARG_MAX + 1];

	i = -1;
	while (line[++i] != '\0' && quotes == false)
	{
		if (line[i] == '$')
		{
			e = herefile_varname(i, var, line);
			i += (int)ft_strlen(var);
			if (e == 1)
				ft_dprintf(fd, "$");
			else
				herefile_expansion(fd, var, ctx);
		}
		else
			write(fd, &line[i], 1);
	}
	if (quotes == true)
		ft_dprintf(fd, "%s", line);
	ft_dprintf(fd, "\n");
}
