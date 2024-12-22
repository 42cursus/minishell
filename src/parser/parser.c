/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 14:42:35 by sbelcher          #+#    #+#             */
/*   Updated: 2024/12/15 14:42:40 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	redir_to_null(t_ast_node *node)
{
	node->cmd->redirects_in = NULL;
	node->cmd->redirects_out = NULL;
	node->cmd->redirects_err = NULL;
	node->cmd->redirects_err_in = NULL;
}

t_ast_node	*create_node(t_node_type type, const char *value, t_ast_node *parent, t_token_type token)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = type;
	node->cmd = malloc(sizeof(t_cmd_node));
	ft_memset(node->cmd, 0, sizeof(t_cmd_node));
	if (type == NODE_COMMAND && value != NULL)
	{
		node->cmd->args = malloc(sizeof(t_wrd)); 
		node->cmd->args->value = strdup(value);
		if (token == TOKEN_VAR)
			node->cmd->args->expand = true;
		else
			node->cmd->args->expand = false;
		node->cmd->args->next_part = NULL;
		node->cmd->args->next_word = NULL;
	}
	else
		node->cmd->args = NULL;
	node->parent = parent;
	redir_to_null(node);
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void list_append(t_wrd **list, t_wrd *redir)
{
	t_wrd *curr;

	if (list == NULL)
		return;
	if (*list == NULL)
		*list = redir;
	else
	{
		curr = *list;
		while (curr->next_word != NULL)
			curr = curr->next_word;
		curr->next_word = redir;
	}
}

void	find_redir_list(t_wrd *redir, t_token_type type, t_cmd_node *cmd)
{
	if (type == TOKEN_REDIRECT_STDOUT || type == TOKEN_APPEND)
		list_append(&cmd->redirects_out, redir);
	else if (type == TOKEN_REDIRECT_STDIN || type == TOKEN_HERE_DOC)
		list_append(&cmd->redirects_in, redir);
	else if (type == TOKEN_REDIRECT_STDERR || type == TOKEN_APPEND_2)
		list_append(&cmd->redirects_err, redir); 
	else if (type == TOKEN_REDIRECT_IN_2 || type == TOKEN_HERE_DOC_2)
		list_append(&cmd->redirects_err_in, redir); 
}

void	parse_redirection(t_token **tokens, int *token_pos, t_ast_node *parent)
{
	t_wrd			*redir;
	t_token_type	rt;

	redir = NULL;
	rt = tokens[*token_pos]->type;
	skip_blanks(tokens, token_pos, NULL);
	if (tokens[*token_pos] && (tokens[*token_pos]->type == TOKEN_WORD || tokens[*token_pos]->type == TOKEN_VAR))
	{
		redir = malloc(sizeof(t_wrd));
		create_wrd(redir, tokens[*token_pos]);
		if (rt == TOKEN_APPEND || rt == TOKEN_APPEND_2)
			redir->append = true;
		skip_blanks(tokens, token_pos, redir);
		find_redir_list(redir, rt, parent->cmd);
	}
	else
		ft_printf("Syntax error: Expected target after redirection\n");
}

t_ast_node	*parse_command(t_token **tokens, int *token_pos)
{
	t_ast_node	*command_node;
	t_wrd		*last_arg;
	t_wrd		*arg;

	if (tokens[*token_pos]->type == TOKEN_WORD || tokens[*token_pos]->type == TOKEN_VAR)
	{
		command_node = create_node(NODE_COMMAND, tokens[*token_pos]->value, NULL, tokens[*token_pos]->type);
		skip_blanks(tokens, token_pos, command_node->cmd->args);
	}
	else
		command_node = create_node(NODE_COMMAND, NULL, NULL, TOKEN_COMMAND);
	if (!command_node)
		return (NULL);
	last_arg = command_node->cmd->args;
	while (tokens[*token_pos] != NULL)
	{
		if (tokens[*token_pos]->type == TOKEN_WORD || tokens[*token_pos]->type == TOKEN_VAR)
		{
			arg = malloc(sizeof(t_wrd));
			create_wrd(arg, tokens[*token_pos]);
			if (command_node->cmd->args == NULL)
				command_node->cmd->args = arg;
			else if (last_arg != NULL)
				last_arg->next_word = arg;
			last_arg = arg;
			skip_blanks(tokens, token_pos, arg);
		}
		else if (tokens[*token_pos]->type >= TOKEN_REDIRECT_STDOUT && tokens[*token_pos]->type < TOKEN_MAX)
			parse_redirection(tokens, token_pos, command_node);
		else
			break;
	}
	return (command_node);
}

void	create_wrd(t_wrd *word, t_token *token)
{
	if (token)
	{
		word->value = ft_strdup(token->value);
		if (token->type == TOKEN_VAR)
			word->expand = true;
		else
			word->expand = false;
	}
	else
	{
		word->value = NULL;
		word->expand = false;
	}
	word->next_part = NULL;
	word->next_word = NULL;
}

void	skip_blanks(t_token **tokens, int *token_pos, t_wrd *last)
{
	(*token_pos)++;
	if (tokens[*token_pos])
	{
		if (last != NULL)
		{
			while (tokens[*token_pos]->type == TOKEN_WORD || tokens[*token_pos]->type == TOKEN_VAR)
			{
				last->next_part = malloc(sizeof(t_wrd));
				create_wrd(last->next_part, tokens[*token_pos]);
				last = last->next_part;
				(*token_pos)++;
				if (!tokens[*token_pos])
					break;
			}
		}
	}
	if (tokens[*token_pos])
	{
		if (tokens[*token_pos]->type == TOKEN_BLANK)
			(*token_pos)++;
	}
}

int has_right(t_lexer *lexer, t_ast_node **right)
{
	if (lexer->tokens[lexer->token_iter] && lexer->tokens[lexer->token_iter]->type == TOKEN_PIPE)
	{
		skip_blanks(lexer->tokens, &lexer->token_iter, NULL);
		*right = parse_command(lexer->tokens, &lexer->token_iter);
		return (true);
	}
	*right = NULL;
	return (false);
}

#define HALF_BAKED_TREE 2135646

int	parse_pipeline(char *line, t_ast_node **root)
{
	t_ast_node	*cn;
	t_ast_node	*nn;
	t_ast_node	*pipe_node;
	t_lexer		lexer;
	int			errcode;

	errcode = scan_the_Line(line, &lexer);
	if (lexer.tokens[lexer.token_iter]->type == TOKEN_BLANK)
		(lexer.token_iter)++;
	if (!errcode)
	{
		cn = parse_command(lexer.tokens, &lexer.token_iter);
		if (cn != NULL)
		{
			while (has_right(&lexer, &nn))
			{
				if (nn)
				{
					pipe_node = create_node(NODE_PIPE, NULL, NULL, TOKEN_PIPE);
					pipe_node->cmd = (free(pipe_node->cmd), NULL);
					if (!pipe_node)
					{
						ft_printf("Error: Memory allocation failed.\n");
						cn = (free_ast(cn), NULL);
						free_ast(nn);
						break;
					}
					pipe_node->left = cn;
					pipe_node->right = nn;
					cn->parent = pipe_node;
					nn->parent = pipe_node;
					cn = pipe_node;
				}
				else
				{
					errcode = HALF_BAKED_TREE;
					break;
				}
			}
		}
		*root = cn;
		free_tokens(&lexer);
	}
	return (errcode);
}

#define BUFF_SIZE 1024

void print_arguments(t_wrd *arguments, int depth)
{
	char buf[BUFF_SIZE];
	t_wrd *node;

	while (arguments)
	{
		*buf = '\0';
		for (int i = 0; i < depth; i++)
			ft_printf("  ");
		if (!arguments->value)
			snprintf(buf, BUFF_SIZE, "NULL");
		else if (arguments->value && arguments->expand)
			snprintf(buf, BUFF_SIZE, "expand(%s)", arguments->value);
		else
			snprintf(buf, BUFF_SIZE, "%s", arguments->value);
		ft_printf("ARGUMENT: %s", buf);
		t_wrd	*cont_node = arguments;
		while (cont_node->next_part)
		{
			*buf = '\0';
			node = cont_node->next_part;
			if (!node->value)
				snprintf(buf, BUFF_SIZE, "NULL");
			else if (node->value && node->expand)
				snprintf(buf, BUFF_SIZE, "expand(%s)", node->value);
			else
				snprintf(buf, 1024, "%s", node->value);
			ft_printf("; %s", buf);
			cont_node = cont_node->next_part;
		}
		printf("\n");
		arguments = arguments->next_word;
	}
}

void	print_redirections(t_wrd *redir, int depth, t_token_type rt)
{
	const char	*type;

	if (rt == TOKEN_REDIRECT_STDOUT)
		type = ">";
	else if (rt == TOKEN_REDIRECT_STDERR)
		type = "2>";
	else if (rt == TOKEN_REDIRECT_STDIN)
		type = "<";
	else if (rt == TOKEN_REDIRECT_IN_2)
		type = "2<";
	while (redir)
	{
		int	i;

		i = -1;
		while (++i < depth)
			ft_printf("  ");
		printf("Redirection Type: %s", type);
		if (redir->append == true)
			printf(">");
		printf(", Target: %s", redir->value);
		if (redir->expand == true)
			printf(" - TO EXPAND");
		while (redir->next_part)
		{
			printf("; %s", redir->next_part->value ? redir->next_part->value : "NULL");
			if (redir->next_part->expand == true)
				printf(" - TO EXPAND");
			redir = redir->next_part;
		}
		printf("\n");
		redir = redir->next_word;
	}
}

void	print_ast(t_ast_node *node, int depth)
{
	int	i;
	i = -1;

	if (!node)
		return;
	while (++i < depth)
		ft_printf("  ");
	if (node->type == NODE_COMMAND)
		printf("COMMAND: ");
	else if (node->type == NODE_PIPE)
		printf("PIPE: ");
	if (node->type == NODE_COMMAND && node->cmd->args)
	{
		printf("%s", node->cmd->args->value);
		if (node->cmd->args->expand == true)
			printf(" - TO EXPAND");
		t_wrd	*cont = node->cmd->args;
		while (cont->next_part)
		{
			printf("; %s", cont->next_part->value ? cont->next_part->value : "NULL");
			if (cont->next_part->expand == true)
				printf(" - TO EXPAND");
			cont = cont->next_part;
		}
	}
	printf("\n");
	if (node->type == NODE_COMMAND)
	{
		if (node->cmd->args->next_word)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Arguments:\n");
			print_arguments(node->cmd->args->next_word, depth + 2);
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
			print_redirections(node->cmd->redirects_out, depth + 2, TOKEN_REDIRECT_STDOUT);
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
	else if (node->type == NODE_PIPE)
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

void	free_wrd(t_wrd *word)
{
	if (word->value)
		free(word->value);
	if (word->next_part)
		free_wrd(word->next_part);
	if (word->next_word)
		free_wrd(word->next_word);
	free(word);
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return;
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
	free(node->cmd);
	free(node);
}
