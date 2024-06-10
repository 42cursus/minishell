/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 14:28:47 by yublee           ###   ########.fr       */
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

void	expand_tree_redirect_l(t_btree *root)
{
	int		i;
	char	*str;
	char	*new;
	t_btree	*current;

	if (!root || !root->item)
		return ;
	str = root->item;
	i = 0;
	while (str[i])
	{
		if (str[i] == '<')
		{
			new = ft_worddup(&str[i], '<');
			current = root;
			while (current->left)
				current = current->left;
			current->left = create_node(new);
			if (str[i + 1] == '<')
				i++;
		}
		i++;
	}
}
//needs malloc protection

void	expand_tree_redirect_r(t_btree *root)
{
	int		i;
	char	*str;
	char	*new;
	t_btree	*current;

	if (!root || !root->item)
		return ;
	str = root->item;
	i = 0;
	while (str[i])
	{
		if (str[i] == '>')
		{
			new = ft_worddup(&str[i], '>');
			printf("new: %s\n", new);
			current = root;
			while (current->right)
				current = current->right;
			current->right = create_node(new);
			if (str[i + 1] == '>')
				i++;
		}
		i++;
	}
}
//needs malloc protection

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

void	ft_strtrim_and_free(t_btree *root)
{
	char	*result;
	result = ft_strtrim(root->item, " ");
	free(root->item);
	root->item = result;
}

int	main(void)
{
	char	*str_init;
	char	*str;
	t_btree	*root;

	// str_init = "ls -l | wc -l < output | ls >> output2";
	str_init = "<i1 <i2 ls -l > output | ls >> output2";
	// str_init = "<<EOF cat | cat > output > output2";
	str = ft_strdup(str_init);
	root = create_node(str);
	expand_tree_pipe(root);
	btree_apply_suffix(root, ft_strtrim_and_free);
	btree_apply_suffix(root, expand_tree_redirect_l);
	btree_apply_suffix(root, expand_tree_redirect_r);
	btree_apply_infix(root, print_node);
	btree_apply_suffix(root, free_node);
}
