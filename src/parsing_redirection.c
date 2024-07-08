/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:00:45 by yublee            #+#    #+#             */
/*   Updated: 2024/07/08 03:38:41 by yublee           ###   ########.fr       */
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

static void	expand_left_right(t_btree *root, char *str_sub, char *str)
{
	size_t	i;
	char	c;
	char	*str_new;

	i = -1;
	while (str_sub[++i])
	{
		if (str_sub[i] == '<' || str_sub[i] == '>')
		{
			c = str_sub[i];
			str_new = ft_worddup(&str[i], c);
			add_left_right(root, c, str_new);
			if (str_sub[i + 1] == c)
				i++;
		}
	}
}

void	expand_tree_redirect(t_btree *root)
{
	char	*str;
	char	*str_sub;
	char	*tmp;
	char	*str_new;

	if (!root || !root->item)
		return ;
	str = root->item;
	str_sub = str_with_substituted_quotes(str, 'c');
	expand_left_right(root, str_sub, str);
	str_new = ft_leftoverdup(str, str_sub, '<');
	tmp = str_new;
	str_new = ft_leftoverdup(str_new, str_sub, '>');
	free(tmp);
	free(root->item);
	free(str_sub);
	root->item = str_new;
}
