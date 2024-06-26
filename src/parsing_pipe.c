/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:07:25 by yublee            #+#    #+#             */
/*   Updated: 2024/06/16 23:42:48 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	split_left_right(t_btree *root, char *str, size_t i)
{
	char	*right;
	char	*left;
	size_t	len;

	right = ft_strdup(&str[i] + 1);
	if (!right)
		exit(EXIT_FAILURE);
	len = ft_strlen(str) - ft_strlen(right);
	left = (char *)malloc(len);
	if (!left)
		exit(EXIT_FAILURE);
	ft_strlcpy(left, str, len);
	root->left = create_node(left);
	root->right = create_node(right);
	free(str);
	root->item = ft_strdup("|");
	if (!root->item)
		exit(EXIT_FAILURE);
	expand_tree_pipe(root->right);
}

void	expand_tree_pipe(t_btree *root)
{
	char	*str;
	char	quote;
	size_t	len;
	size_t	i;

	str = root->item;
	if (!ft_strchr(str, '|'))
		return ;
	len = ft_strlen(str);
	i = -1;
	while (++i < len)
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			while (str[++i] != quote && i < len)
				;
			i++;
		}
		if (str[i] == '|')
		{
			split_left_right(root, str, i);
			return ;
		}
	}
}
