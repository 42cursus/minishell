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

t_ast_node	*create_node(t_node_type type, t_token *t, t_ast_node *parent)
{
	t_ast_node	*node;

	node = ft_calloc(sizeof(t_ast_node), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = ft_calloc(sizeof(t_cmd_node), 1);
	if (type == NODE_COMMAND && t != NULL)
	{
		node->cmd->args = ft_calloc(sizeof(t_wrd), 1);
		node->cmd->args->value = strdup(t->value);
		if (t->type == TOKEN_VAR)
			node->cmd->args->expand = true;
	}
	node->parent = parent;
	return (node);
}

void	list_append(t_wrd **list, t_wrd *redir)
{
	t_wrd	*curr;

	if (list == NULL)
		return ;
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

char	*hd_cat_loop(t_wrd *here, size_t len)
{
	char	*str;

	str = ft_calloc(sizeof(char), len + 1);
	while (here)
	{
		ft_strcat(str, here->value);
		here = here->next_part;
	}
	return (str);
}

void	here_doc_cat(t_wrd *here)
{
	t_wrd		*origin;
	char		*str;
	size_t		len;

	origin = here;
	len = 0;
	if (here->next_part)
	{
		while (here)
		{
			len += ft_strlen(here->value);
			here = here->next_part;
		}
		here = origin;
		str = hd_cat_loop(here, len);
		free((void *)origin->value);
		free_wrd(origin->next_part);
		origin->value = ft_strdup(str);
		free(str);
		origin->next_part = NULL;
	}
}

int	add_random_numbers_to_str(char *str_buf, int rand_count)
{
	char	random_code[6];
	char	buf[1];
	int		fd;
	int		i;
	int		ret_val;

	ret_val = 0;
	ft_strncpy(str_buf, "/tmp/heredock_", FILENAME_BUF_SIZE);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd >= 0)
	{
		i = 0;
		while (read(fd, buf, 1) && i < rand_count)
			if (ft_isalnum(buf[0]))
				random_code[i++] = buf[0];
		close(fd);
		random_code[i] = '\0';
		ft_strcat(str_buf, random_code);
	}
	else
	{
		ret_val = -1;
		errno = COULDNT_OPEN_URANDOM;
	}
	return (ret_val);
}

int	create_here_file(t_wrd *here, HeredocEntry *entry)
{
	int	error_code;

	error_code = 0;
	error_code = add_random_numbers_to_str(entry->filename, 5);
	if (!error_code)
	{
		ft_strcpy(entry->delimiter, here->value);
		free((void *)here->value);
		here->value = NULL;
		here->value = ft_strdup(entry->filename);
	}
	else
		perror("Couldn't add_random_numbers_to_str");
	return (error_code);
}

void	parse_redirection(t_token **t, int *tp, t_ast_node *parent, t_ctx *ctx)
{
	t_wrd			*redir;
	t_token_type	rt;
	HeredocEntry	*entry;

	rt = t[*tp]->type;
	skip_blanks(t, tp, NULL);
	if (t[*tp] && (t[*tp]->type == TOKEN_WORD || t[*tp]->type == TOKEN_VAR))
	{
		redir = ft_calloc(sizeof(t_wrd), 1);
		create_wrd(redir, t[*tp], rt);
		skip_blanks(t, tp, redir);
		if (rt == TOKEN_HERE_DOC || rt == TOKEN_HERE_DOC_2)
		{
			here_doc_cat(redir);
			entry = &ctx->hd.entries[ctx->hd.ss++];
			create_here_file(redir, entry);
		}
		find_redir_list(redir, rt, parent->cmd);
	}
	else
		ft_printf("Syntax error: Expected target after redirection\n");
}

void	parse_command_loop(t_token **t, int *tp, t_ctx *ctx, t_ast_node *cn)
{
	t_wrd	*arg;
	t_wrd	*la;

	la = cn->cmd->args;
	while (t[*tp] != NULL)
	{
		if (t[*tp]->type == TOKEN_WORD || t[*tp]->type == TOKEN_VAR)
		{
			arg = ft_calloc(sizeof(t_wrd), 1);
			create_wrd(arg, t[*tp], t[*tp]->type);
			if (cn->cmd->args == NULL)
				cn->cmd->args = arg;
			else if (la != NULL)
				la->next_word = arg;
			la = arg;
			skip_blanks(t, tp, arg);
		}
		else if (t[*tp]->type >= TOKEN_REDIRECT_STDOUT && t[*tp]->type < 16)
			parse_redirection(t, tp, cn, ctx);
		else
			break ;
	}
}

t_ast_node	*parse_command(t_token **t, int *tp, t_ctx *ctx)
{
	t_ast_node	*command_node;

	if (t[*tp]->type == TOKEN_WORD || t[*tp]->type == TOKEN_VAR)
	{
		command_node = create_node(NODE_COMMAND, t[*tp], NULL);
		skip_blanks(t, tp, command_node->cmd->args);
	}
	else
		command_node = create_node(NODE_COMMAND, NULL, NULL);
	if (!command_node)
		return (NULL);
	parse_command_loop(t, tp, ctx, command_node);
	return (command_node);
}

void	create_wrd(t_wrd *word, t_token *token, t_token_type rt)
{
	ft_memset(word, 0, sizeof(t_wrd));
	if (token)
	{
		word->value = ft_strdup(token->value);
		if (token->type == TOKEN_VAR)
			word->expand = true;
		else
			word->expand = false;
	}
	if (rt == TOKEN_APPEND || rt == TOKEN_APPEND_2)
		word->append = true;
}

void	skip_blanks(t_token **ts, int *tp, t_wrd *last)
{
	t_token	*t;

	if (ts[++(*tp)] && last != NULL)
	{
		while (ts[*tp]->type == TOKEN_WORD || ts[*tp]->type == TOKEN_VAR)
		{
			last->next_part = ft_calloc(sizeof(t_wrd), 1);
			create_wrd(last->next_part, ts[*tp], ts[*tp]->type);
			last = last->next_part;
			if (!ts[++(*tp)])
				break ;
		}
	}
	if (ts[*tp])
	{
		t = ts[*tp];
		while ((*tp < 1024) && t && t->type == TOKEN_BLANK)
			t = ts[++(*tp)];
	}
}

int	has_right(t_lexer *l, t_ast_node **right, t_ctx *ctx)
{
	t_token_type	type;

	type = l->tokens[l->token_iter]->type;
	if (l->tokens[l->token_iter] && type == TOKEN_PIPE)
	{
		skip_blanks(l->tokens, &l->token_iter, NULL);
		*right = parse_command(l->tokens, &l->token_iter, ctx);
		return (true);
	}
	*right = NULL;
	return (false);
}

t_ast_node	*pipeline_loop(t_lexer *lexer, t_ctx *ctx)
{
	t_ast_node	*cn;
	t_ast_node	*nn;
	t_ast_node	*pipe_node;

	cn = parse_command(lexer->tokens, &lexer->token_iter, ctx);
	if (cn != NULL)
	{
		while (has_right(lexer, &nn, ctx))
		{
			pipe_node = create_node(NODE_PIPE, NULL, NULL);
			pipe_node->cmd = (free(pipe_node->cmd), NULL);
			pipe_node->left = cn;
			pipe_node->right = nn;
			cn->parent = pipe_node;
			nn->parent = pipe_node;
			cn = pipe_node;
		}
	}
	return (cn);
}

int	parse_pipeline(const char *line, t_ast_node **root, t_ctx *ctx)
{
	t_lexer		lexer;
	int			errcode;

	errcode = scan_the_line(line, &lexer);
	if (lexer.tokens[lexer.token_iter]->type == TOKEN_BLANK)
		skip_blanks(lexer.tokens, &lexer.token_iter, NULL);
	if (!errcode)
	{
		*root = pipeline_loop(&lexer, ctx);
		free_tokens(&lexer);
	}
	ctx->hd.size = ctx->hd.ss;
	ctx->hd.ss = 0;
	return (errcode);
}

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
	free(node->cmd);
	free(node);
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

		
		t_wrd *arg = node->cmd->args;
		if (arg && arg->next_word)
		{
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
