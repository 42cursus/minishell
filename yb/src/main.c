/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/06/08 14:23:13 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	expand_tree(t_btree *root)
{
	char	*right;
	char	*left;
	size_t	len;

	if (!ft_strchr(root->item, '|'))
		return ;
	else
	{
		right = ft_strdup(ft_strchr(root->item, '|') + 1);
		len = ft_strlen(root->item) - ft_strlen(right);
		left = (char *)malloc(len);
		ft_strlcpy(left, root->item, len);
		root->left = create_node(left);
		root->right = create_node(right);
		free(root->item);
		root->item = ft_strdup("|");
		expand_tree(root->right);
	}
}

t_btree	*create_node(void *item)
{
	t_btree	*node;

	node = (t_btree *)malloc(sizeof(t_btree));
	node->left = NULL;
	node->right = NULL;
	node->item = item;
	return (node);
}

// void	btree_print_infix(t_btree *root)
// {
// 	if (root == NULL)
// 		return ;
// 	btree_print_infix(root->left);
// 	ft_printf("%s\n", root->item);
// 	btree_print_infix(root->right);
// }

void	btree_free(t_btree *root)
{
	if (root == NULL)
		return ;
	btree_free(root->left);
	btree_free(root->right);
	free(root->item);
	free(root);
}

void	strtrim_with_free(void *str)
{
	// char	*tmp;

	// tmp = str;
	// str = ft_strtrim(str, " ");
	// if (ft_strlen(tmp) != ft_strlen(str))
	// 	free(tmp);
	ft_printf("%s\n", str);
}

void	btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
	if (root == NULL)
		return ;
	btree_apply_infix(root->left, applyf);
	applyf(root->item);
	btree_apply_infix(root->right, applyf);
}

int	main()
{
	char	*str_init;
	char	*str;
	t_btree	*root;

	str_init = "ls -a | ls -b | ls -c | ls -d"; // can be modified
	str = ft_strdup(str_init);
	root = create_node(str);
	expand_tree(root);
	btree_apply_infix(root, strtrim_with_free);
	btree_free(root);
}