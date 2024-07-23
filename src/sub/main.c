/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:04:07 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 14:04:08 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <history.h>
#include <readline.h>
#include "minishell.h"
#include "test.h"

void	ft_sh_init_welcome(void);
int		ft_sh_execute(t_obj_arr *ops, t_ctx *ctx);

int ft_sh_execute_yb(t_obj_arr *ops, char *line, t_ctx *ctx)
{
	char	*str;
	t_btree	*root;
	t_list	*cmd_list;

	(void)ops;
	syntax_validation(line);

	str = ft_strdup(line);
	if (!str)
		return (-1);
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
	pipex(&cmd_list, root, ctx, ops);

	//free only when testing
	// btree_apply_suffix(root, free_node);
	return (0);
}

int ft_sh_loop(t_ctx *ctx, t_obj_arr *ops)
{
	char	*line;
	int		status;

	ft_sh_init_welcome();
	status = 0;
//	if (ctx->argc > 1)
//		ctx->argv = ft_split(ctx->argv[1], ' ');
//	status = ft_sh_execute(ops, ctx);
	ctx->argv = NULL;
	using_history();    /* initialize history */
	while (!status)
	{
		line = ft_sh_read_line(ctx);
		if (line)
		{
			if (*line != 0)
			{
				add_history(line);
//				if (ft_sh_split_line(line, ctx))
//					break;
				status = ft_sh_execute_yb(ops, line, ctx);
			}
			else
				free(line);
		}
		else
			printf("\n");
	}
	return (status);
}

/**
 * https://brennan.io/2015/01/16/write-a-shell-in-c/
 * https://www.geeksforgeeks.org/making-linux-shell-c/
 */
int main(int argc, char **argv, char **envp)
{
	int 		exitcode;
	t_ctx		*global;
	t_obj_arr	*ops;

	if (ft_sh_do_init(&global, envp, &ops) != -1)
	{
		global->argv = argv;
		global->argc = argc;
		global->envp = envp;
		exitcode = ft_sh_loop(global, ops);
	}
	else
		exitcode = EXIT_FAILURE;
	ft_sh_destroy_ctx(global);
	global = NULL;
	return (exitcode);
}
