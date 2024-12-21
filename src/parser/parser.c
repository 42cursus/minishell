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
	node->cmd->redirects_err_append = NULL;
	node->cmd->redirects_err_here_doc = NULL;
	node->cmd->redirects_here_doc = NULL;
	node->cmd->redirects_out_append = NULL;
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
	if (value)
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

int	end_of_redir_list(t_wrd *redir, t_wrd *list, t_token_type type, t_cmd_node *cmd)
{
	if (list == NULL)
	{
		if (type == TOKEN_REDIRECT_STDOUT)
			cmd->redirects_out = redir;
		else if (type == TOKEN_REDIRECT_STDIN)
			cmd->redirects_in  = redir;
		else if (type == TOKEN_APPEND)
			cmd->redirects_out_append  = redir;
		else if (type == TOKEN_HERE_DOC)
			cmd->redirects_here_doc  = redir;
		else if (type == TOKEN_REDIRECT_STDERR)
			cmd->redirects_err  = redir;
		else if (type == TOKEN_REDIRECT_IN_2)
			cmd->redirects_err_in  = redir;
		else if (type == TOKEN_HERE_DOC_2)
			cmd->redirects_err_here_doc = redir;
		else if (type == TOKEN_APPEND_2)
			cmd->redirects_err_append = redir;
		return (0);
	}
	while (list->next_word != NULL)
		list = list->next_word;
	list->next_word = redir;
	return (0);
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
	return (0);
}

void	find_redir_list(t_wrd *redir, t_token_type type, t_cmd_node *cmd)
{
	if (type == TOKEN_REDIRECT_STDOUT)
		list_append(&cmd->redirects_out, redir);
	else if (type == TOKEN_REDIRECT_STDIN || TOKEN_HERE_DOC)
		end_of_redir_list(redir, cmd->redirects_in, type, cmd);
	else if (type == TOKEN_APPEND)
		end_of_redir_list(redir, cmd->redirects_out_append, type, cmd);
	else if (type == TOKEN_HERE_DOC)
	{

		list_append(&cmd->redirects_here_doc, redir);

	}
		
	else if (type == TOKEN_REDIRECT_STDERR)
		end_of_redir_list(redir, cmd->redirects_err, type, cmd); 
	else if (type == TOKEN_REDIRECT_IN_2)
		end_of_redir_list(redir, cmd->redirects_err_in, type, cmd); 
	else if (type == TOKEN_HERE_DOC_2)
		end_of_redir_list(redir, cmd->redirects_err_here_doc, type, cmd);
	else if (type == TOKEN_APPEND_2)
		end_of_redir_list(redir, cmd->redirects_err_append, type, cmd);
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
		word->value = token->value;
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
	//TODO: replace me with a while loop inside scan_the_Line
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
						//echo current_node = (free_ast(current_node), NULL);
						//free_ast(next_command);
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
					ft_printf("Syntax error: Expected command after '|'\n");
					//current_node = (free_ast(current_node), NULL);
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

/*void print_arguments(t_ast_node *arg_node, int depth)
{
	char buf[BUFF_SIZE];
	t_ast_node *node;

	while (arg_node)
	{
		*buf = '\0';
		for (int i = 0; i < depth; i++)
			ft_printf("  ");
		if (!arg_node->value)
			snprintf(buf, BUFF_SIZE, "NULL");
		else if (arg_node->value && arg_node->expand)
			snprintf(buf, BUFF_SIZE, "expand(%s)", arg_node->value);
		else
			snprintf(buf, BUFF_SIZE, "%s", arg_node->value);
		ft_printf("ARGUMENT: %s", buf);

		t_ast_node	*cont_node = arg_node;
		while (cont_node->word_continue)
		{
			*buf = '\0';
			node = cont_node->word_continue;
			if (!node->value)
				snprintf(buf, BUFF_SIZE, "NULL");
			else if (node->value && node->expand)
				snprintf(buf, BUFF_SIZE, "expand(%s)", node->value);
			else
				snprintf(buf, 1024, "%s", node->value);
			ft_printf("; %s", buf);
			cont_node = cont_node->word_continue;
		}
		printf("\n");
		arg_node = arg_node->arguments;
	}
}

void	print_redirections(t_ast_node *redir_node, int depth)
{
	while (redir_node)
	{
		int	i;

		i = -1;
		while (++i < depth)
			ft_printf("  ");
		printf("Redirection Type: %s, Target: %s",
			   redir_node->value, redir_node->right->value);
		if (redir_node->right->expand == true)
			printf(" - TO EXPAND");
		t_ast_node	*file = redir_node->right;
		while (file->word_continue)
		{
			printf("; %s", file->word_continue->value ? file->word_continue->value : "NULL");
			if (file->word_continue->expand == true)
				printf(" - TO EXPAND");
			file = file->word_continue;
		}
		printf("\n");
		if (redir_node->type == NODE_REDIRECT_STDIN || redir_node->type == NODE_HERE_DOC || redir_node->type == NODE_REDIRECT_IN_2 || redir_node->type == NODE_HERE_DOC_2)
			redir_node = redir_node->redirects_in;
		else if (redir_node->type == NODE_REDIRECT_STDOUT || redir_node->type == NODE_APPEND || redir_node->type == NODE_REDIRECT_STDERR || redir_node->type == NODE_APPEND_2)
			redir_node = redir_node->redirects_out;
		else
			redir_node = NULL;
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
	printf("%s: %s",
		   node->type == NODE_COMMAND ? "COMMAND" :
		   node->type == NODE_ARGUMENT ? "ARGUMENT" :
		   node->type == NODE_PIPE ? "PIPE" :
		   node->type == NODE_REDIRECT_STDOUT ? "REDIRECT_OUT" :
		   node->type == NODE_REDIRECT_STDIN ? "REDIRECT_IN" :
		   node->type == NODE_APPEND ? "APPEND" :
		   node->type == NODE_HERE_DOC ? "HERE_DOC" :
		   node->type == NODE_FILENAME ? "FILENAME" :
		   "UNKNOWN",
		   node->value ? node->value : "NULL");
	if (node->expand == true)
		printf(" - TO EXPAND");
	t_ast_node	*cont = node;
	while (cont->word_continue)
	{
		printf("; %s", cont->word_continue->value ? cont->word_continue->value : "NULL");
		if (cont->word_continue->expand == true)
			printf(" - TO EXPAND");
		cont = cont->word_continue;
	}
	printf("\n");
	if (node->type == NODE_COMMAND)
	{
		if (node->arguments)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Arguments:\n");
			print_arguments(node->arguments, depth + 2);
		}
		if (node->redirects_in)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Input Redirections:\n");
			print_redirections(node->redirects_in, depth + 2);
		}
		if (node->redirects_out)
		{
			i = -1;
			while (++i < depth + 1)
				ft_printf("  ");
			ft_printf("Output Redirections:\n");
			print_redirections(node->redirects_out, depth + 2);
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

void	free_arguments(t_ast_node *arg_node)
{
	while (arg_node)
	{
		t_ast_node *temp = arg_node;
		free(arg_node->value);
		arg_node = arg_node->arguments;
		free(temp);
	}
}

void	free_redirections(t_ast_node *redir_node)
{
	while (redir_node)
	{
		t_ast_node	*temp;

		temp = redir_node;
		free(redir_node->value);
		if (redir_node->right)
			free_ast(redir_node->right);
		if (redir_node->type == NODE_REDIRECT_STDIN || redir_node->type == NODE_HERE_DOC)
			redir_node = redir_node->redirects_in;
		else if (redir_node->type == NODE_REDIRECT_STDOUT || redir_node->type == NODE_APPEND)
			redir_node = redir_node->redirects_out;
		else
			redir_node = NULL;
		free(temp);
	}
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return;
	free(node->value);
	if (node->arguments)
		free_arguments(node->arguments);
	if (node->redirects_in)
		free_redirections(node->redirects_in);
	if (node->redirects_out)
		free_redirections(node->redirects_out);
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	free(node);
}*/
