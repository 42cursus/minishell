/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child_process.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 00:34:13 by yublee            #+#    #+#             */
/*   Updated: 2024/07/10 02:20:23 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "test.h"

int child_process(int i, t_list *current, t_info info)
{
	t_btree	*cmd;
	char	*cmd_str;
	char	*tmp;
	char	**argv;
	int 	ret_val;
	t_shell_op	*op;

	ret_val = 0;
	cmd = (t_btree *)current->content;
	get_input(cmd, i, info);
	get_output(cmd, i, info);
	cmd_str = cmd->item;
	if (cmd_str[0] == 0)
		exit_with_message(NULL, EXIT_SUCCESS, info);
	argv = get_argv(cmd_str, info.env);
	op = ft_bsearch_obj(&(t_shell_op){.instruction = argv[0]}, info.ops);
	if (op != NULL)
	{
		info.ctx->argv = argv;
		info.ctx->argc = (int)ft_tab_get_size((const void **) argv);
		ret_val = op->fun(info.ctx);
	}
	else
	{
		if (access(argv[0], X_OK))
		{
			tmp = ft_strdup(argv[0]);
			free_str_array(argv);
			exit_with_message(tmp, 127, info);
		}
		if (execve(argv[0], argv, info.env) == -1)
		{
			free_str_array(argv);
			exit_with_message("execve", EXIT_FAILURE, info);
		}
	}
	return (ret_val);
}
