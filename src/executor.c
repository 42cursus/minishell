/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:17:44 by abelov            #+#    #+#             */
/*   Updated: 2024/12/18 18:17:45 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "cmd.h"

int exec_ast(t_ast_node *command, int level, t_ast_node *parent)
{
	if (command == NULL)
		return (SHELL_EXIT);

	if (command->type == NODE_COMMAND)
	{

	}

	return 0;
}

