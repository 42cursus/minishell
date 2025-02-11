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

int	ft_getpid_c(void)
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
		if (l->tok.t[l->tok.token_iter] != NULL)
		{
			*r = parse_command(l->tok.t, &l->tok.token_iter, ctx, l);
			if (*r && l->err == 0)
				return (true);
		}
		else
			l->err = OP_OUT_OF_PLACE;
	}
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
		cn->ctx = ctx;
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
		"Syntax Error: Operator out of place.\n",
		"Error: Failed to concatenate HERE DOC delimiter.\n",
		"Syntax Error: Unsupported '&' operator.\n",
	};

	if (errcode == EMPTY_LINE)
		return (0);
	if (errcode == TOKEN_ALLOC_FAILURE)
		ft_dprintf(STDERR_FILENO, lt[errcode], lexer->line_iter);
	else
		ft_dprintf(STDERR_FILENO, lt[errcode]);
	return (lexer->err);
}
