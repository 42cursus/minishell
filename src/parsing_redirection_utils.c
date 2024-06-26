/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:00:45 by yublee            #+#    #+#             */
/*   Updated: 2024/06/25 17:41:39 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	skip_word(char *str, size_t i, char c)
{
	i++;
	if (str[i] == c)
		i++;
	while (str[i] == ' ')
		i++;
	while (str[i] && str[i] != ' ')
		i++;
	return (i);
}

static size_t	excluded_len(char *str, char c)
{
	size_t	i;
	size_t	j;
	size_t	cnt;
	char	quote;

	cnt = 0;
	i = 0;
	while (str[i])
	{
		j = 1;
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			while (str[++i] && str[i] != quote)
				;
			i++;
		}
		if (str[i] == c)
		{
			j = skip_word(str, i, c) - i - 1;
			cnt += j;
		}
		i += j;
	}
	return (cnt);
}

static void	cpy_leftover(char *str, char *result, size_t len, char c)
{
	size_t	i;
	size_t	j;
	char	quote;

	i = 0;
	j = 0;
	while (j < len)
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			while (str[i])
			{
				result[j++] = str[i++];
				if (str[i] == quote)
					break ;
			}
			result[j++] = str[i++];
		}
		if (str[i] == c)
			i = skip_word(str, i, c);
		result[j++] = str[i++];
	}
	result[j] = 0;
}

char	*ft_leftoverdup(char *str, char c)
{
	size_t	len;
	char	*result;

	if (!excluded_len(str, c))
	{
		result = ft_strdup(str);
		return (result);
	}
	len = ft_strlen(str) - excluded_len(str, c);
	result = (char *)malloc(len + 1);
	if (!result)
		exit(EXIT_FAILURE);
	cpy_leftover(str, result, len, c);
	return (result);
}

char	*ft_worddup(char *str, char c)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*result;

	i = 1;
	j = 0;
	if (str[i] == c)
		i++;
	while (str[i++] == ' ')
		j++;
	while (str[i] && str[i] != ' ')
		i++;
	len = i - j;
	result = (char *)malloc(len + 1);
	if (!result)
		exit(EXIT_FAILURE);
	i = -1;
	j = 0;
	while (j < len)
		if (str[++i] != ' ')
			result[j++] = str[i];
	result[j] = 0;
	return (result);
}