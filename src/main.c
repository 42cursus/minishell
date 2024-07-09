/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/07/09 16:40:57 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*str;
	t_btree	*root;
	t_list	*cmd_list;

	(void)argc;
	(void)env;
	open("args.txt", O_TRUNC | O_CREAT, 0777); //only for test
	if (argv[1])
	{
		syntax_validation(argv[1]);

		str = ft_strdup(argv[1]);
		if (!str)
			exit(EXIT_FAILURE);
		root = create_node(str);

		// create tree according to pipe
		expand_tree_pipe(root);

		// create subtree according to redirection
		btree_apply_suffix(root, expand_tree_redirect);

		// trim spaces
		btree_apply_suffix(root, ft_strtrim_and_free);

		// print tree to check
		// btree_apply_infix(root, print_node);
		// printf("----------------------------\n");

		// get the command list according to pipe
		cmd_list = get_cmds(root);

		// print list to check
		// ft_lstiter(cmd_list, print_list);

		// pass cmd list to pipex
		pipex(&cmd_list, root, env);

		//free only when testing
		// btree_apply_suffix(root, free_node);
	}
}
