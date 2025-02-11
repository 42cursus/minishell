/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:12:47 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/11 13:12:50 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_wrd(t_wrd *word)
{
	if (word->value)
		free((void *)word->value);
	if (word->next_part)
		free_wrd(word->next_part);
	if (word->next_word)
		free_wrd(word->next_word);
	free(word);
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		if (node->cmd->args)
			free_wrd(node->cmd->args);
		if (node->cmd->redirects_in)
			free_wrd(node->cmd->redirects_in);
		if (node->cmd->redirects_out)
			free_wrd(node->cmd->redirects_out);
		if (node->cmd->redirects_err_in)
			free_wrd(node->cmd->redirects_err_in);
		if (node->cmd->redirects_err)
			free_wrd(node->cmd->redirects_err);
	}
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	if (node->cmd)
		free(node->cmd);
	free(node);
}
