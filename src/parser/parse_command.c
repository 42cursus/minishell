/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:06:50 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/11 13:06:52 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		while ((*tp < MAX_TOKENS) && t && t->type == TOKEN_BLANK)
			t = ts[++(*tp)];
	}
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

	command_node = NULL;
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
	else if (t[*tp]->type >= TOKEN_OR && t[*tp]->type <= TOKEN_PIPE)
		l->err = OP_OUT_OF_PLACE;
	else
		command_node = create_node(NODE_COMMAND, NULL, NULL);
	if (!command_node)
		return (NULL);
	parse_command_loop(tp, ctx, command_node, l);
	return (command_node);
}

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
