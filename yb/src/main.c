/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 18:08:54 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*str_init;
	char	*str;
	t_btree	*root;
	t_list	*cmd_list;

	(void)argc;
	(void)argv;
	(void)env;
	// str_init = "ls -l | wc -l < output | ls >> output2";
	// str_init = "<i1 <i2 ls -l > output | ls >> output2";
	// str_init = "<<EOF cat | cat > output > output2";
	str_init = "<<EOF cat1 | <input1 >output1 | <input2 cat2 >output2 >>output3";
	str = ft_strdup(str_init);
	root = create_node(str);
	// create tree according to pipe
	expand_tree_pipe(root);
	// create subtree according to redirction
	btree_apply_suffix(root, expand_tree_redirect_l);
	btree_apply_suffix(root, expand_tree_redirect_r);
	// trim spaces
	btree_apply_suffix(root, ft_strtrim_and_free);
	// print tree to check
	btree_apply_infix(root, print_node);
	// get the command list according to pipe
	cmd_list = get_cmds(root);
	// print list to check
	ft_lstiter(cmd_list, print_list);
	// pass cmd list to pipex
	pipex(cmd_list, env);
	// free list
	ft_lstclear(&cmd_list, del);
	// free tree
	btree_apply_suffix(root, free_node);
}
