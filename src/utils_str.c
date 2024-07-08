/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 23:56:28 by yublee            #+#    #+#             */
/*   Updated: 2024/07/08 02:59:57 by yublee           ###   ########.fr       */
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

// static size_t	ft_wordcount(char const *s, char c)
// {
// 	size_t	count;
// 	size_t	i;

// 	count = 0;
// 	i = 0;
// 	while (s[i])
// 	{
// 		while (s[i] == c)
// 			i++;
// 		if (s[i])
// 		{
// 			count++;
// 			while (s[i] && s[i] != c)
// 				i++;
// 		}
// 	}
// 	return (count);
// }

// static char	*ft_copytil(char const *s, char c)
// {
// 	size_t	len;
// 	size_t	i;
// 	char	*result;

// 	len = 0;
// 	i = 0;
// 	while (s[len] != c && s[len])
// 		len++;
// 	result = malloc(len + 1);
// 	if (!result)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		result[i] = s[i];
// 		i++;
// 	}
// 	result[i] = '\0';
// 	return (result);
// }

// char	**ft_split_str_with_quotes(char *s, char c)
// {
// 	char	*s_tmp;
// 	size_t	i;
// 	char	quote;
// 	char	c_sub;

// 	s_tmp = ft_strdup(s);
// 	i = 0;
// 	c_sub = c + 1;
// 	if (!c_sub)
// 		c_sub++;
// 	while (s[i])
// 	{
// 		if (s[i] == '\'' || s[i] == '"')
// 		{
// 			quote = s[i];
// 			s_tmp[i] = c_sub;
// 			while (s[++i] && s[i] != quote)
// 				s_tmp[i] = c_sub;
// 			i++;
// 			s_tmp[i] = c_sub;
// 		}
// // 		// if ()
// 		i++;
// 	}	
// }

char	*str_with_substituted_quotes(char *s, char c)
{
	char	*s_tmp;
	size_t	i;
	char	quote;
	char	c_sub;

	s_tmp = ft_strdup(s);
	if (!s_tmp)
		exit(EXIT_FAILURE);
	i = 0;
	c_sub = c + 1;
	if (!c_sub)
		c_sub++;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			quote = s[i];
			s_tmp[i] = c_sub;
			while (s[++i] && s[i] != quote)
				s_tmp[i] = c_sub;
			s_tmp[i] = c_sub;
		}
		i++;
	}
	return (s_tmp);
}