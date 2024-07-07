/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 23:56:28 by yublee            #+#    #+#             */
/*   Updated: 2024/07/08 00:55:23 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strtrim_and_free(t_btree *root)
{
	char	*result;

	if (ft_strlen(root->item) > 0)
	{
		result = ft_strtrim(root->item, " ");
		free(root->item);
		root->item = result;
	}
}

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*ft_copytil(char const *s, char c)
{
	size_t	len;
	size_t	i;
	char	*result;

	len = 0;
	i = 0;
	while (s[len] != c && s[len])
		len++;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	while (i < len)
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	**ft_split_str_with_quotes(char const *s, char c)
{
	size_t	wordcount;
	size_t	i;
	size_t	j;
	char	**result;

	if (!s)
		return (NULL);
	wordcount = ft_wordcount(s, c);
	result = (char **)malloc((wordcount + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < wordcount)
	{
		while (s[j] == c)
			j++;
		if (s[j])
			result[i] = ft_copytil(&s[j], c);
		while (s[j] != c && s[j])
			j++;
		i++;
	}
	result[i] = NULL;
	return (result);
}
