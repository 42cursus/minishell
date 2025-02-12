/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:11:29 by abelov            #+#    #+#             */
/*   Updated: 2025/01/21 17:11:29 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ast(t_ast_node *node, int depth)
{
	int	i;

	if (!node)
		return;
	i = -1;
	while (++i < depth)
		ft_printf("  ");
	if (node->type == NODE_COMMAND)
		ft_printf("COMMAND: ");
	else if (node->type == NODE_PIPE)
		ft_printf("PIPE: ");
	else if (node->type == NODE_OR)
		ft_printf("OR ||: ");
	else if (node->type == NODE_AND)
		ft_printf("AND &&: ");
	else if (node->type == NODE_DUMMY)
		ft_printf("DUMMY (): ");
	if (node->type == NODE_COMMAND)
	{
		t_wrd *arg = node->cmd->args;
		if (arg)
		{
			ft_print_arg_node(node->cmd->args);
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Arguments:\n");
			print_arguments(arg->next_word, depth + 2);
		}
		if (node->cmd->redirects_in)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Input Redirections:\n");
			print_redirections(node->cmd->redirects_in, depth + 2, TOKEN_REDIRECT_STDIN);
		}
		if (node->cmd->redirects_out)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Output Redirections:\n");
			print_redirections(node->cmd->redirects_out, depth + 2, T_REDIRECT_STDOUT);
		}
		if (node->cmd->redirects_err_in)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Error Input Redirections:\n");
			print_redirections(node->cmd->redirects_err_in, depth + 2, TOKEN_REDIRECT_IN_2);
		}
		if (node->cmd->redirects_err)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Error Output Redirections:\n");
			print_redirections(node->cmd->redirects_err, depth + 2, TOKEN_REDIRECT_STDERR);
		}
	}
	else if (node->type == NODE_PIPE || node->type == NODE_OR || node->type == NODE_AND)
	{
		if (node->left)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Left Command ->\n");
			print_ast(node->left, depth + 2);
		}
		if (node->right)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Right Command ->\n");
			print_ast(node->right, depth + 2);
		}
	}
}
