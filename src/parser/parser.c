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
		if (t->type == T_VAR)
			node->cmd->args->expand = true;
	}
	node->parent = parent;
	return (node);
}

static void	list_append(t_wrd **list, t_wrd *redir)
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
	if (type == T_REDIRECT_STDOUT || type == TOKEN_APPEND)
		list_append(&cmd->redirects_out, redir);
	else if (type == TOKEN_REDIRECT_STDIN || type == TOKEN_HERE_DOC)
		list_append(&cmd->redirects_in, redir);
	else if (type == TOKEN_REDIRECT_STDERR || type == TOKEN_APPEND_2)
		list_append(&cmd->redirects_err, redir);
	else if (type == TOKEN_REDIRECT_IN_2 || type == TOKEN_HERE_DOC_2)
		list_append(&cmd->redirects_err_in, redir);
}

char	*hd_cat_loop(t_wrd *here, size_t len, t_lexer *l)
{
	char	*str;

	str = ft_calloc(sizeof(char), len + 1);
	if (str == NULL)
		l->err = ALLOC_FAILURE;
	while (here)
	{
		ft_strcat(str, here->value);
		here = here->next_part;
	}
	return (str);
}

int	here_doc_cat(t_wrd *here, t_lexer *l)
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
		str = hd_cat_loop(here, len, l);
		if (!str)
			return (HD_CAT_FAILURE);
		free((void *)origin->value);
		free_wrd(origin->next_part);
		origin->value = ft_strdup(str);
		free(str);
		origin->next_part = NULL;
	}
	return (0);
}

void	remove_non_compliant_chars(char *buf, int buf_size)
{
	char	*dst;

	dst = buf;
	while (buf_size--)
	{
		if (ft_isalnum((unsigned char) *buf))
			*dst++ = *buf;
		buf++;
	}
	*dst = '\0';
	return ;
}

/**
 *
 * @param str_buf
 * @param rand_count
 * @return
 */
static int	add_random_numbers_to_str(char *str_buf, int rand_count)
{
	char	buf[OPTIMISTIC + 1];
	int		fd;
	ssize_t	bytes_read;
	int		len;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		errno = COULDNT_OPEN_URANDOM;
		return (-1);
	}
	len = 0;
	bytes_read = 1;
	while (rand_count > len && bytes_read > 0)
	{
		bytes_read = read(fd, buf + len, OPTIMISTIC - len);
		remove_non_compliant_chars(buf + len, OPTIMISTIC - len);
		len = (int)ft_strnlen(buf, OPTIMISTIC);
	}
	close(fd);
	ft_strncat(str_buf, buf, rand_count);
	return (EX_OK);
}

__attribute__((unused))
static int	ft_getpid_c(void)
{
	int		fd;
	char	buf[20];
	int		i;
	size_t	bytes_read;

	i = 0;
	ft_bzero(buf, 20);
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	bytes_read = read(fd, buf, 15);
	if (bytes_read <= 0)
		return (-1);
	close(fd);
	while (ft_isdigit(buf[i]) && buf[i] && buf[i] != '\0')
		i++;
	buf[i] = '\0';
	return (ft_atoi(buf));
}

static int	create_here_file(t_wrd *here, t_hd_entry *entry, bool expand)
{
	int		olderrno;
	ssize_t	error_code;

	olderrno = errno;
	error_code = 0;
	if (here)
	{
		entry->quotes = expand;
		ft_snprintf(entry->filename, FILENAME_BUF_SIZE,
			"/tmp/heredoc_%d_", ft_getpid());
		add_random_numbers_to_str(entry->filename, 20);
		error_code = errno;
		ft_strcpy(entry->delimiter, here->value);
		free((void *)here->value);
		here->value = NULL;
		here->value = ft_strdup(entry->filename);
	}
	errno = olderrno;
	return ((int)error_code);
}

void	parse_redirection(int *tp, t_ast_node *p, t_ctx *ctx, t_lexer *l)
{
	t_wrd			*redir;
	t_token_type	rt;
	bool			hereexpand;
	t_hd_entry		*entry;

	rt = l->tok.t[*tp]->type;
	hereexpand = l->tok.t[*tp]->hereexpand;
	skip_blanks(l->tok.t, tp, NULL, l);
	if (l->tok.t[*tp]
		&& (l->tok.t[*tp]->type == T_WORD || l->tok.t[*tp]->type == T_VAR))
	{
		redir = ft_calloc(sizeof(t_wrd), 1);
		create_wrd(redir, l->tok.t[*tp], rt);
		skip_blanks(l->tok.t, tp, redir, l);
		if (rt == TOKEN_HERE_DOC || rt == TOKEN_HERE_DOC_2)
		{
			l->err = here_doc_cat(redir, l);
			entry = &ctx->hd.entries[ctx->hd.ss++];
			if (l->err == 0)
				l->err = create_here_file(redir, entry, hereexpand);
		}
		find_redir_list(redir, rt, p->cmd);
	}
	else
		l->err = NO_REDIR_TARGET;
}

void	parse_command_loop(int *tp, t_ctx *ctx, t_ast_node *cn, t_lexer *l)
{
	t_wrd	*arg;
	t_wrd	*la;

	la = cn->cmd->args;
	while (l->tok.t[*tp] != NULL && l->err == 0)
	{
		if (l->tok.t[*tp]->type == T_WORD || l->tok.t[*tp]->type == T_VAR)
		{
			arg = ft_calloc(sizeof(t_wrd), 1);
			create_wrd(arg, l->tok.t[*tp], l->tok.t[*tp]->type);
			if (cn->cmd->args == NULL)
				cn->cmd->args = arg;
			else if (la != NULL)
				la->next_word = arg;
			la = arg;
			skip_blanks(l->tok.t, tp, arg, l);
		}
		else if (l->tok.t[*tp]->type >= T_REDIRECT_STDOUT
			&& l->tok.t[*tp]->type < 16)
			parse_redirection(tp, cn, ctx, l);
		else if (l->tok.t[*tp]->type == TOKEN_DUMMY)
			l->err = UNEXPECTED_DUMMY;
		else
			break ;
	}
}

t_ast_node	*parse_command(t_token **t, int *tp, t_ctx *ctx, t_lexer *l)
{
	t_ast_node	*command_node;

	if (t[*tp]->type == T_WORD || t[*tp]->type == T_VAR)
	{
		command_node = create_node(NODE_COMMAND, t[*tp], NULL);
		skip_blanks(t, tp, command_node->cmd->args, l);
	}
	else if (t[*tp]->type == TOKEN_DUMMY)
	{
		command_node = create_node(NODE_DUMMY, t[*tp], NULL);
		free(command_node->cmd);
		command_node->cmd = NULL;
		skip_blanks(t, tp, NULL, l);
		return (command_node);
	}
	else
		command_node = create_node(NODE_COMMAND, NULL, NULL);
	if (!command_node)
		return (NULL);
	parse_command_loop(tp, ctx, command_node, l);
	return (command_node);
}

void	create_wrd(t_wrd *word, t_token *token, t_token_type rt)
{
	ft_memset(word, 0, sizeof(t_wrd));
	if (token)
	{
		word->value = ft_strdup(token->value);
		if (token->type == T_VAR)
			word->expand = true;
	}
	word->redir_flag = O_TRUNC;
	if (rt == TOKEN_APPEND || rt == TOKEN_APPEND_2)
		word->redir_flag = O_APPEND;
}

void	skip_blanks(t_token **ts, int *tp, t_wrd *last, t_lexer *l)
{
	t_token	*t;

	if (ts[++(*tp)] && last != NULL)
	{
		while (ts[*tp]->type == T_WORD || ts[*tp]->type == T_VAR)
		{
			last->next_part = ft_calloc(sizeof(t_wrd), 1);
			if (!last->next_part)
				l->err = ALLOC_FAILURE;
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

static int	has_right(t_lexer *l, t_ast_node **r, t_ctx *ctx, t_node_type *type)
{
	t_token_type	t;

	t = TOKEN_END;
	if (l->tok.t[l->tok.token_iter] != NULL)
		t = l->tok.t[l->tok.token_iter]->type;
	if (l->tok.t[l->tok.token_iter] && (t >= TOKEN_OR && t <= TOKEN_PIPE))
	{
		if (t == TOKEN_OR)
			*type = NODE_OR;
		else if (t == TOKEN_AND)
			*type = NODE_AND;
		else if (t == TOKEN_PIPE)
			*type = NODE_PIPE;
		skip_blanks(l->tok.t, &l->tok.token_iter, NULL, l);
		*r = parse_command(l->tok.t, &l->tok.token_iter, ctx, l);
		return (true);
	}
	else if (t != TOKEN_END)
		l->err = UNEXPECTED_DUMMY;
	*r = NULL;
	return (false);
}

t_ast_node	*pipeline_loop(t_lexer *lexer, t_ctx *ctx)
{
	t_ast_node	*cn;
	t_ast_node	*nn;
	t_ast_node	*op_node;
	t_node_type	type;

	cn = parse_command(lexer->tok.t, &lexer->tok.token_iter, ctx, lexer);
	if (cn != NULL)
	{
		while (has_right(lexer, &nn, ctx, &type) && lexer->err == 0)
		{
			op_node = create_node(type, NULL, NULL);
			free(op_node->cmd);
			op_node->cmd = NULL;
			op_node->left = cn;
			op_node->right = nn;
			cn->parent = op_node;
			nn->parent = op_node;
			cn = op_node;
		}
	}
	return (cn);
}

int	handle_parser_err(int errcode, t_lexer *lexer)
{
	const char	*lt[SINGLE_AMPERSAND + 1] = {
		"",
		"Syntax Error: Unexpected () in input.\n",
		"Error: Failure to open /dev/urandom.\n",
		"Error: Input contained an un-closed quote.\n",
		"Error: Token allocation failure near position[%d].\n",
		"Error: PID allocation failure.\n",
		"Error: Memory allocation failure.\n",
		"Error: Reidirection without a target.\n",
		"Syntax Error: Operator at line beginning.\n",
		"Error: Failed to concatenate HERE DOC delimiter.\n",
		"Syntax Error: Unsupported '&' operator.\n",
	};

	if (errcode == EMPTY_LINE)
		errcode = EX_OK;
	else
	{
		if (errcode == TOKEN_ALLOC_FAILURE)
			ft_dprintf(STDERR_FILENO, lt[errcode], lexer->line_iter);
		else
			ft_dprintf(STDERR_FILENO, lt[errcode]);
		errcode = EX_BADUSAGE;
	}
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
	if (node->cmd)
		free(node->cmd);
	free(node);
}

#define BUFF_SIZE 1024

static void	ft_print_arg_node(t_wrd *arguments, char *buf)
{
	t_wrd	*cont_node;
	t_wrd	*node;

	cont_node = arguments;
	while (cont_node->next_part)
	{
		*buf = '\0';
		node = cont_node->next_part;
		if (node->value && node->expand)
			ft_snprintf(buf, BUFF_SIZE, "expand(%s)", node->value);
		else if (node->value && ft_strlen(node->value) == 0)
			ft_snprintf(buf, BUFF_SIZE, "(empty string)");
		else
			ft_snprintf(buf, BUFF_SIZE, "%s", node->value);
		ft_printf("; %s", buf);
		cont_node = cont_node->next_part;
	}
	printf("\n");
}

void	print_arguments(t_wrd *arguments, int depth)
{
	char	buf[BUFF_SIZE];
	int		i;

	while (arguments)
	{
		*buf = '\0';
		i = -1;
		while (++i < depth)
			ft_printf("  ");
		if (arguments->value && ft_strlen(arguments->value) == 0)
			ft_snprintf(buf, BUFF_SIZE, "(empty string)");
		else if (arguments->value && arguments->expand)
			ft_snprintf(buf, BUFF_SIZE, "expand(%s)", arguments->value);
		else
			ft_snprintf(buf, BUFF_SIZE, "%s", arguments->value);
		ft_printf("ARGUMENT: %s", buf);
		ft_print_arg_node(arguments, buf);
		arguments = arguments->next_word;
	}
}

void	print_redirections(t_wrd *redir, int depth, t_token_type rt)
{
	const char	*type;
	int			i;

	if (rt == T_REDIRECT_STDOUT)
		type = ">";
	else if (rt == TOKEN_REDIRECT_STDERR)
		type = "2>";
	else if (rt == TOKEN_REDIRECT_STDIN)
		type = "<";
	else if (rt == TOKEN_REDIRECT_IN_2)
		type = "2<";
	while (redir)
	{
		i = -1;
		while (++i < depth)
			ft_printf("  ");
		printf("Redirection Type: %s", type);
		if (redir->redir_flag == O_APPEND)
			printf(">");
		printf(", Target: %s", redir->value);
		if (redir->expand == true)
			printf(" - TO EXPAND");
		while (redir->next_part)
		{
			if (redir->next_part->value)
				printf("; %s", redir->next_part->value);
			else
				printf("; %s", "NULL");
			if (redir->next_part->expand == true)
				printf(" - TO EXPAND");
			redir = redir->next_part;
		}
		printf("\n");
		redir = redir->next_word;
	}
}
