/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:00:45 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 18:10:58 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_worddup(char *str, char c)
{
	int		i;
	int		j;
	int		len;
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
		exit(EXIT_FAILUARE);
	i = -1;
	j = 0;
	while (j < len && ++i >= 0)
		if (str[i] != ' ')
			result[j++] = str[i];
	result[j] = 0;
	return (result);
}

static int	excluded_len(char *str, char c)
{
	int	i;
	int	j;
	int	cnt;

	cnt = 0;
	i = 0;
	while (str[i])
	{
		j = 1;
		if (str[i] == c)
		{
			if (str[i + j] == c)
				j++;
			while (str[i + j] == ' ')
				j++;
			while (str[i + j] && str[i + j] != ' ')
				j++;
			cnt += j;
		}
		i += j;
	}
	return (cnt);
}

static char	*ft_leftoverdup(char *str, char c)
{
	int		i;
	int		j;
	int		len;
	char	*result;

	len = ft_strlen(str) - excluded_len(str, c);
	result = (char *)malloc(len + 1);
	if (!result)
		exit(EXIT_FAILUARE);
	i = 0;
	j = 0;
	while (j < len + 1)
	{
		if (str[i] == c)
		{
			if (str[++i] == c)
				i++;
			while (str[i] == ' ')
				i++;
			while (str[i] && str[i] != ' ')
				i++;
		}
		result[j++] = str[i++];
	}
	return (result);
}

void	expand_tree_redirect_l(t_btree *root)
{
	int		i;
	char	*str;
	char	*str_new;
	t_btree	*current;

	if (!root || !root->item)
		return ;
	str = root->item;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '<')
		{
			str_new = ft_worddup(&str[i], '<');
			current = root;
			while (current->left)
				current = current->left;
			current->left = create_node(str_new);
			if (str[i + 1] == '<')
				i++;
		}
	}
	str_new = ft_leftoverdup(str, '<');
	free(root->item);
	root->item = str_new;
}

void	expand_tree_redirect_r(t_btree *root)
{
	int		i;
	char	*str;
	char	*str_new;
	t_btree	*current;

	if (!root || !root->item)
		return ;
	str = root->item;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '>')
		{
			str_new = ft_worddup(&str[i], '>');
			current = root;
			while (current->right)
				current = current->right;
			current->right = create_node(str_new);
			if (str[i + 1] == '>')
				i++;
		}
	}
	str_new = ft_leftoverdup(str, '>');
	free(root->item);
	root->item = str_new;
}
