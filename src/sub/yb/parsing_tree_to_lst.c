/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tree_to_lst.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:34:25 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 17:46:05 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_cmds(t_list **head, t_btree *root)
{
	t_list	*new;
	t_btree	*current;
	char	*str;

	current = root->right;
	while (current)
	{
		str = (char *)current->item;
		if (str[0] != '|')
		{
			new = ft_lstnew(current);
			ft_lstadd_back(head, new);
			return ;
		}
		new = ft_lstnew(current->left);
		ft_lstadd_back(head, new);
		current = current->right;
	}
}

t_list	*get_cmds(t_btree *root)
{
	char	*str;
	t_list	*head;

	str = (char *)root->item;
	if (str && str[0] != '|')
		head = ft_lstnew(root);
	else
	{
		head = ft_lstnew(root->left);
		add_cmds(&head, root);
	}
	return (head);
}
