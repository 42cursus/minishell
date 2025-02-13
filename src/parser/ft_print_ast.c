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

static void	ft_print_depth(int depth)
{
	int	i;

	i = -1;
	while (++i < depth + 1)
		ft_printf("  ");
}

void	print_redirections(t_wrd *redir, int depth, t_token_type rt)
{
	const char	*type;
	const char	*lt[TOKEN_MAX][2] = {
	[TOKEN_REDIRECT_STDIN] = {"Input Redirections:\n", "<"},
	[T_REDIRECT_STDOUT] = {"Output Redirections:\n", ">"},
	[TOKEN_REDIRECT_IN_2] = {"Error Input Redirections:\n", "2<"},
	[TOKEN_REDIRECT_STDERR] = {"Error Output Redirections:\n", "2>"},
	};

	ft_print_depth(depth);
	ft_printf(lt[rt][0]);
	type = lt[rt][1];
	while (redir)
	{
		ft_print_depth(depth + 2);
		ft_printf("Redirection Type: %s", type);
		if (redir->redir_flag == O_APPEND)
			ft_printf(">");
		ft_printf(", Target: ");
		ft_print_arg_node(redir);
		redir = redir->next_word;
	}
}

static void	ft_sh_print_cmd_node(t_ast_node *node, int depth)
{
	t_wrd	*arg;

	arg = node->cmd->args;
	if (arg)
	{
		ft_print_arg_node(node->cmd->args);
		ft_print_depth(depth);
		ft_printf("Arguments:\n");
		print_arguments(arg->next_word, depth + 2);
	}
	if (node->cmd->redirects_in)
		print_redirections(node->cmd->redirects_in, depth,
			TOKEN_REDIRECT_STDIN);
	if (node->cmd->redirects_out)
		print_redirections(node->cmd->redirects_out, depth,
			T_REDIRECT_STDOUT);
	if (node->cmd->redirects_err_in)
		print_redirections(node->cmd->redirects_err_in, depth,
			TOKEN_REDIRECT_IN_2);
	if (node->cmd->redirects_err)
		print_redirections(node->cmd->redirects_err, depth,
			TOKEN_REDIRECT_STDERR);
}

static void	ft_sh_print_complex(t_ast_node *node, int depth)
{
	ft_printf("\n");
	if (node->left)
	{
		ft_print_depth(depth);
		ft_printf("Left Command ->\n");
		print_ast(node->left, depth + 2);
	}
	if (node->right)
	{
		ft_print_depth(depth);
		ft_printf("Right Command ->\n");
		print_ast(node->right, depth + 2);
	}
}

void	print_ast(t_ast_node *node, int depth)
{
	if (!node)
		return ;
	ft_print_depth(depth);
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
		ft_sh_print_cmd_node(node, depth);
	else if (node->type == NODE_PIPE
		|| node->type == NODE_OR || node->type == NODE_AND)
		ft_sh_print_complex(node, depth);
}
