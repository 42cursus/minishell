/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 12:29:04 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	find_special_char(char *item)
{
	int	i;

	i = 0;
	while (item && item[i])
	{
		if (item[i] == '<')
		{
			if (item[++i] == '<')
				return ('d');
			else
				return ('<');
		}
		else if (item[i] == '>')
		{
			if (item[++i] == '>')
				return ('a');
			else
				return ('>');
		}
		i++;
	}
	return (0);
}
// d indicates << (deliminator)
// a indicates >> (append mode)

void	expand_tree_redirect(t_btree *root)
{
	char	*right;
	char	*left;
	char	c;
	int		offset;
	size_t	left_len;

	c = find_special_char(root->item);
	if (!c)
		return ;
	offset = 1;
	if (c == 'd' || c == 'a')
		offset++;
	if (c == '<' || c == 'd')
		right = ft_strdup(ft_strchr(root->item, '<') + offset);
	else
		right = ft_strdup(ft_strchr(root->item, '>') + offset);
	left_len = ft_strlen(root->item) - ft_strlen(right) - offset + 1;
	left = (char *)malloc(left_len);
	ft_strlcpy(left, root->item, left_len);
	root->left = create_node(left);
	root->right = create_node(right);
	free(root->item);
	root->item = ft_strdup(&c);
}
//needs malloc protection
//several redirections not handled yet

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
		len = ft_strlen(root->item) - ft_strlen(right);
		left = (char *)malloc(len);
		ft_strlcpy(left, root->item, len);
		root->left = create_node(left);
		root->right = create_node(right);
		free(root->item);
		root->item = ft_strdup("|");
		expand_tree_pipe(root->right);
	}
}
//needs malloc protection

int	main(void)
{
	char	*str_init;
	char	*str;
	t_btree	*root;

	str_init = "ls -l | wc -l < output | ls >> output2";
	// str_init = "<<EOF cat | cat > output > output2";
	str = ft_strdup(str_init);
	root = create_node(str);
	expand_tree_pipe(root);
	btree_apply_suffix(root, expand_tree_redirect);
	btree_apply_infix(root, print_node);
	btree_apply_suffix(root, free_node);
}
