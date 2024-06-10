/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 15:07:13 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main(void)
{
	char	*str_init;
	char	*str;
	t_btree	*root;

	// str_init = "ls -l | wc -l < output | ls >> output2";
	// str_init = "<i1 <i2 ls -l > output | ls >> output2";
	str_init = "<<EOF cat | cat > output > output2";
	str = ft_strdup(str_init);
	root = create_node(str);
	expand_tree_pipe(root);
	btree_apply_suffix(root, expand_tree_redirect_l);
	btree_apply_suffix(root, expand_tree_redirect_r);
	btree_apply_suffix(root, ft_strtrim_and_free);
	btree_apply_infix(root, print_node);
	btree_apply_suffix(root, free_node);
}
