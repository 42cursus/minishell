/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:58:21 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/10 16:58:24 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_of_heredoc_check(t_lexer *lexer)
{
	lexer->here_eof = false;
	lexer->first_blank = false;
}

t_token	*create_token(t_token_type type, const char *value, t_lexer *lexer)
{
	t_token	*token;

	if (type == TOKEN_HERE_DOC || type == TOKEN_HERE_DOC_2)
	{
		lexer->here_eof = true;
		lexer->heredoc_index = lexer->tok.token_iter;
	}
	token = ft_calloc(sizeof(t_token), 1);
	if (!token)
	{
		lexer->err = TOKEN_ALLOC_FAILURE;
		return (NULL);
	}
	token->type = type;
	if (value)
		token->value = strdup(value);
	if (!token->value)
	{
		lexer->err = TOKEN_ALLOC_FAILURE;
		free(token);
		return (NULL);
	}
	lexer->tok.t[lexer->tok.token_iter++] = token;
	return (token);
}

void	flush_buffer(t_lexer *lexer, t_token_type type)
{
	if (lexer->buf_index > 0)
	{
		lexer->buffer[lexer->buf_index] = '\0';
		create_token(type, lexer->buffer, lexer);
		lexer->buf_index = 0;
	}
}

void	free_tokens(t_lexer *lexer)
{
	int	i;

	i = -1;
	while (lexer->tok.t[++i] != NULL)
	{
		free(lexer->tok.t[i]->value);
		free(lexer->tok.t[i]);
		lexer->tok.token_iter = 0;
	}
}

t_state	exit_variable(t_lexer *l, t_ctx *ctx)
{
	char	*sc;

	if (l->line[(l->line_iter)] == '?' && l->buf_index == 0)
	{
		(l->line_iter)++;
		sc = ft_itoa(ctx->last_status_code);
		create_token(T_WORD, sc, l);
		free(sc);
	}
	else if (l->buf_index > 0)
		flush_buffer(l, T_VAR);
	if (l->curent_string == '"')
	{
		l->curent_string = '0';
		return (handle_in_double_quote(l, ctx));
	}
	if (l->line[l->line_iter] != '\0')
		return (handle_initial(l, ctx));
	return (INITIAL);
}
