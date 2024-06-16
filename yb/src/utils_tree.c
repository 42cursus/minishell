/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 18:56:33 by yublee            #+#    #+#             */
/*   Updated: 2024/06/16 23:27:47 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_node(void *item)
{
	printf("%s\n", (char *)item);
}

void	free_node(t_btree *root)
{
	free(root->item);
	free(root);
}

t_btree	*create_node(void *item)
{
	t_btree	*node;

	node = (t_btree *)malloc(sizeof(t_btree));
	if (!node)
		exit(EXIT_FAILURE);
	node->left = NULL;
	node->right = NULL;
	node->item = item;
	return (node);
}

void	btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
	if (root == NULL)
		return ;
	btree_apply_infix(root->left, applyf);
	applyf(root->item);
	btree_apply_infix(root->right, applyf);
}

void	btree_apply_suffix(t_btree *root, void (*applyf)(t_btree *))
{
	if (root == NULL)
		return ;
	btree_apply_suffix(root->left, applyf);
	btree_apply_suffix(root->right, applyf);
	applyf(root);
}
