/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <sbelcher@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:20:53 by sbelcher          #+#    #+#             */
/*   Updated: 2024/12/05 14:18:21 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse_redirection(t_token **tokens, int *token_pos)
{
	t_ast_node	*redir_node;

	redir_node = malloc(sizeof(t_ast_node));
	if (!redir_node)
		return (NULL);
	if (tokens[*token_pos]->type == TOKEN_REDIRECT_IN)
		redir_node->type = NODE_REDIRECT_IN;
	else if (tokens[*token_pos]->type == TOKEN_REDIRECT_OUT)
		redir_node->type = NODE_REDIRECT_OUT;
	else if (tokens[*token_pos]->type == TOKEN_REDIRECT_ERR)
		redir_node->type = NODE_REDIRECT_ERR;
	else if (tokens[*token_pos]->type == TOKEN_APPEND)
		redir_node->type = NODE_APPEND;
	else if (tokens[*token_pos]->type == TOKEN_HERE_DOC)
		redir_node->type = NODE_HERE_DOC;
	else
		return (NULL);
	redir_node->value = ft_strdup(tokens[*token_pos]->value);
	redir_node->left = NULL;
	redir_node->next = NULL;
	(*token_pos)++;
	if (tokens[*token_pos] && tokens[*token_pos]->type == TOKEN_WORD)
	{
		redir_node->right = malloc(sizeof(t_ast_node));
		if (!redir_node->right)
		{
			free_ast(redir_node);
			return (NULL);
		}
		redir_node->right->type = NODE_FILENAME;
		redir_node->right->value = ft_strdup(tokens[*token_pos]->value);
		redir_node->right->left = NULL;
		redir_node->right->right = NULL;
		redir_node->right->next = NULL;
		(*token_pos)++;
	}
	else
	{
		ft_printf("Syntax error: Expected target after redirection\n");
		free_ast(redir_node);
		return (NULL);
	}
	return (redir_node);
}

t_ast_node	*parse_command_ast(t_token **tokens, int *token_pos)
{
	t_ast_node	*command_node;

	if (!tokens[*token_pos] || tokens[*token_pos]->type != TOKEN_WORD)
		return (NULL);
	command_node = malloc(sizeof(t_ast_node));
	if (!command_node)
		return (NULL);
	command_node = create_node(NODE_COMMAND, tokens[*token_pos]->value, NULL, NULL, NULL);
	(*token_pos)++;
	t_ast_node *last_arg = NULL;
	t_ast_node *last_redir = NULL;
	while (tokens[*token_pos])
	{
		if (tokens[*token_pos]->type == TOKEN_WORD)
		{
			t_ast_node *arg_node = malloc(sizeof(t_ast_node));
			if (!arg_node)
			{
				free_ast(command_node);
				return (NULL);
			}
			arg_node = create_node(NODE_ARGUMENT, tokens[*token_pos]->value, NULL, NULL, NULL);
			if (!command_node->left)
				command_node->left = arg_node;
			else
				last_arg->next = arg_node;
			last_arg = arg_node;
			(*token_pos)++;
		}
		else if (tokens[*token_pos]->type == TOKEN_REDIRECT_IN ||
				   tokens[*token_pos]->type == TOKEN_REDIRECT_OUT ||
				   tokens[*token_pos]->type == TOKEN_APPEND ||
				   tokens[*token_pos]->type == TOKEN_HERE_DOC)
		{
			t_ast_node *redir_node = parse_redirection(tokens, token_pos);
			if (!redir_node)
			{
				free_ast(command_node);
				return (NULL);
			}
			if (!command_node->right)
				command_node->right = redir_node;
			else
				last_redir->next = redir_node;
			last_redir = redir_node;
		}
		else
			break;
	}
	return (command_node);
}

t_ast_node *create_node(t_node_type type, const char *value, t_ast_node *left, t_ast_node	*right, t_ast_node *next)
{
	t_ast_node	*node;
 
	node = malloc(sizeof(t_ast_node));
	node->type = type;
	node->value = strdup(value);
	node->left = left;
	node->right = right;
	node->next = next;
	return (node);
}

t_ast_node *parse_pipeline(t_token **tokens, int *token_pos)
{
	t_ast_node *command_node = parse_command_ast(tokens, token_pos);
	if (!command_node)
		return (NULL);
	while (tokens[*token_pos] && tokens[*token_pos]->type == TOKEN_PIPE)
	{
		(*token_pos)++;
		t_ast_node *next_command = parse_command_ast(tokens, token_pos);
		if (!next_command)
		{
			ft_printf("Syntax error: Expected command after '|'\n");
			free_ast(command_node);
			return (NULL);
		}
		t_ast_node	*pipe_node = malloc(sizeof(t_ast_node));
		if (!pipe_node)
		{
			ft_printf("Error: Memory allocation failed.\n");
			free_ast(command_node);
			free_ast(next_command);
			return (NULL);
		}
		pipe_node = create_node(NODE_PIPE, "|", command_node, next_command, NULL);
		command_node = pipe_node;
	}
	return (command_node);
}

void	print_redirections(t_ast_node *redir_node, int depth)
{
	while (redir_node)
	{
		for (int i = 0; i < depth; i++)
			ft_printf("  ");
		printf("Redirection Type: %s, Target: %s\n",
			   redir_node->type == NODE_REDIRECT_IN ? "INPUT" :
			   redir_node->type == NODE_REDIRECT_OUT ? "OUTPUT" :
			   redir_node->type == NODE_APPEND ? "APPEND" :
			   redir_node->type == NODE_HERE_DOC ? "HEREDOC" : "UNKNOWN",
			   redir_node->right && redir_node->right->value ? redir_node->right->value : "NULL");
		redir_node = redir_node->next;
	}
}

void	print_arguments(t_ast_node *arg_node, int depth)
{
	while (arg_node)
	{
		for (int i = 0; i < depth; i++)
			ft_printf("  ");
		ft_printf("ARGUMENT: %s\n", arg_node->value ? arg_node->value : "NULL");
		arg_node = arg_node->next;
	}
}

void print_ast(t_ast_node *node, int depth)
{
	if (!node)
		return;
	for (int i = 0; i < depth; i++)
		printf("  ");
	printf("%s: %s\n",
		   node->type == NODE_COMMAND ? "COMMAND" :
		   node->type == NODE_ARGUMENT ? "ARGUMENT" :
		   node->type == NODE_PIPE ? "PIPE" :
		   node->type == NODE_REDIRECT_OUT ? "REDIRECT_OUT" :
		   node->type == NODE_REDIRECT_IN ? "REDIRECT_IN" :
		   node->type == NODE_APPEND ? "APPEND" :
		   node->type == NODE_HERE_DOC ? "HERE_DOC" :
		   node->type == NODE_FILENAME ? "FILENAME" :
		   "UNKNOWN",
		   node->value ? node->value : "NULL");

	if (node->type == NODE_COMMAND)
	{
		if (node->left)
		{
			for (int i = 0; i < depth + 1; i++)
				printf("  ");
			printf("Arguments:\n");
			print_arguments(node->left, depth + 2);
		}
		if (node->right)
		{
			for (int i = 0; i < depth + 1; i++)
				printf("  ");
			printf("Redirections:\n");
			print_redirections(node->right, depth + 2);
		}
	}
	else if (node->type == NODE_PIPE)
	{
		if (node->left)
		{
			for (int i = 0; i < depth + 1; i++)
				printf("  ");
			printf("Left Command ->\n");
			print_ast(node->left, depth + 2);
		}
		if (node->right)
		{
			for (int i = 0; i < depth + 1; i++)
				printf("  ");
			printf("Right Command ->\n");
			print_ast(node->right, depth + 2);
		}
	}

	if (node->next)
		print_ast(node->next, depth);
}

void	free_arguments(t_ast_node *arg_node)
{
	while (arg_node)
	{
		t_ast_node *temp = arg_node;
		free(arg_node->value);
		arg_node = arg_node->next;
		free(temp);
	}
}


void	free_redirections(t_ast_node *redir_node)
{
	while (redir_node)
	{
		t_ast_node *temp = redir_node;
		free(redir_node->value);
		free_ast(redir_node->right);
		redir_node = redir_node->next;
		free(temp);
	}
}

void free_ast(t_ast_node *node)
{
	if (!node)
		return;
	free(node->value);
	if (node->left)
		free_arguments(node->left);
	if (node->right)
	{
		if (node->type == NODE_COMMAND)
			free_redirections(node->right);
		else
			free_ast(node->right);
	}
	free(node);
}
