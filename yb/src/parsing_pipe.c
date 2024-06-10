/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:07:25 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 18:54:38 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_tree_pipe(t_btree *root)
{
	char	*right;
	char	*left;
	size_t	len;

	if (!ft_strchr(root->item, '|'))
		return ;
	else
	{
		right = ft_strdup(ft_strchr(root->item, '|') + 1);
		if (!right)
			exit(EXIT_FAILURE);
		len = ft_strlen(root->item) - ft_strlen(right);
		left = (char *)malloc(len);
		if (!left)
			exit(EXIT_FAILURE);
		ft_strlcpy(left, root->item, len);
		root->left = create_node(left);
		root->right = create_node(right);
		free(root->item);
		root->item = ft_strdup("|");
		if (!root->item)
			exit(EXIT_FAILURE);
		expand_tree_pipe(root->right);
	}
}
