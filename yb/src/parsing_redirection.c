/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:00:45 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 15:07:50 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static char	*ft_worddup(char *str, char c)
{
	int		i;
	int		j;
	int		cnt;
	int		len;
	char	*result;

	i = 1;
	cnt = 0;
	if (str[i] == c)
		i++;
	while (str[i++] == ' ')
		cnt++;
	while (str[i] && str[i] != ' ')
		i++;
	len = i - cnt;
	result = (char *)malloc(len + 1);
	i = 0;
	j = 0;
	while (j < len)
	{
		if (str[i] != ' ')
			result[j++] = str[i];
		i++;
	}
	result[j] = 0;
	return (result);
}
//needs malloc protection

static char	*ft_leftoverdup(char *str, char c)
{
	int		i;
	int		j;
	int		cnt;
	int		len;
	char	*result;

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
	len = ft_strlen(str) - cnt;
	result = (char *)malloc(len + 1);
	i = 0;
	j = 0;
	while (j < len)
	{
		if (str[i] == c)
		{
			i++;
			if (str[i] == c)
				i++;
			while (str[i] == ' ')
				i++;
			while (str[i] && str[i] != ' ')
				i++;
		}
		result[j++] = str[i++];
	}
	result[j] = 0;
	return (result);
}

void	expand_tree_redirect_l(t_btree *root)
{
	int		i;
	char	*str;
	char	*str_left;
	char	*str_new;
	t_btree	*current;

	if (!root || !root->item)
		return ;
	str = root->item;
	i = 0;
	while (str[i])
	{
		if (str[i] == '<')
		{
			str_left = ft_worddup(&str[i], '<');
			current = root;
			while (current->left)
				current = current->left;
			current->left = create_node(str_left);
			if (str[i + 1] == '<')
				i++;
		}
		i++;
	}
	str_new = ft_leftoverdup(str, '<');
	free(root->item);
	root->item = str_new;
}
//needs malloc protection

void	expand_tree_redirect_r(t_btree *root)
{
	int		i;
	char	*str;
	char	*str_right;
	char	*str_new;
	t_btree	*current;

	if (!root || !root->item)
		return ;
	str = root->item;
	i = 0;
	while (str[i])
	{
		if (str[i] == '>')
		{
			str_right = ft_worddup(&str[i], '>');
			current = root;
			while (current->right)
				current = current->right;
			current->right = create_node(str_right);
			if (str[i + 1] == '>')
				i++;
		}
		i++;
	}
	str_new = ft_leftoverdup(str, '>');
	free(root->item);
	root->item = str_new;
}
//needs malloc protection