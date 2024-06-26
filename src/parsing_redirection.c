/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:00:45 by yublee            #+#    #+#             */
/*   Updated: 2024/06/25 18:03:52 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_left_right(t_btree *root, char c, char *str_new)
{
	t_btree	*current;

	current = root;
	if (c == '<')
	{
		while (current->left)
			current = current->left;
		current->left = create_node(str_new);
	}
	else if (c == '>')
	{
		while (current->right)
			current = current->right;
		current->right = create_node(str_new);
	}
}

static void	expand_left_right(t_btree *root, char *str)
{
	size_t	i;
	char	quote;
	char	c;
	char	*str_new;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			while (str[++i] && str[i] != quote)
				;
			i++;
		}
		if (str[i] == '<' || str[i] == '>')
		{
			c = str[i];
			str_new = ft_worddup(&str[i], c);
			add_left_right(root, c, str_new);
			if (str[i + 1] == c)
				i++;
		}
	}
}

void	expand_tree_redirect(t_btree *root)
{
	char	*str;
	char	*tmp;
	char	*str_new;

	if (!root || !root->item)
		return ;
	str = root->item;
	expand_left_right(root, str);
	str_new = ft_leftoverdup(str, '<');
	tmp = str_new;
	str_new = ft_leftoverdup(str_new, '>');
	free(tmp);
	free(root->item);
	root->item = str_new;
}