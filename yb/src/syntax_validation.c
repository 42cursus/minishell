/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 18:57:05 by yublee            #+#    #+#             */
/*   Updated: 2024/06/16 23:41:14 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	first_char(char *str)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(str);
	i = 0;
	while (i < len && str[i] == ' ')
		i++;
	if (i == len)
		return (0);
	return (str[i]);
}

static int	balance_check(char *str)
{
	size_t	len;
	size_t	i;
	char	quote;

	if (!str)
		return (-1);
	len = ft_strlen(str);
	i = 0;
	while (i < len)
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			while (str[++i] != quote && i < len)
				;
			if (i == len)
				return (-1);
		}
		i++;
	}
	return (0);
}

void	syntax_validation(char *str)
{
	if (first_char(str) == '|' || balance_check(str) < 0)
	{
		write(2, "Syntax error\n", ft_strlen("Syntax error\n"));
		exit(EXIT_FAILURE);
	}
}
