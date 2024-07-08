/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:00:45 by yublee            #+#    #+#             */
/*   Updated: 2024/07/08 04:03:00 by yublee           ###   ########.fr       */
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

	cnt = 0;
	i = 0;
	while (str[i])
	{
		j = 0;
		if (str[i] == c)
		{
			j = skip_word(str, i, c) - i;
			cnt += j;
			if (str[i + 1] == c)
				i++;
		}
		i++;
	}
	return (cnt);
}

char	*ft_leftoverdup(char *str, char *str_sub, char c)
{
	size_t	len;
	char	*result;
	size_t	i;
	size_t	j;

	if (!excluded_len(str_sub, c))
	{
		result = ft_strdup(str);
		return (result);
	}
	len = ft_strlen(str) - excluded_len(str_sub, c);
	result = (char *)malloc(len + 1);
	if (!result)
		exit(EXIT_FAILURE);
	i = 0;
	j = 0;
	while (j < len)
	{
		if (str_sub[i] == c)
			i = skip_word(str_sub, i, c);
		result[j++] = str[i++];
	}
	result[j] = 0;
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
