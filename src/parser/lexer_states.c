/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_states.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:21:14 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/10 16:21:19 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_state	handle_initial(t_lexer *l, t_ctx *ctx)
{
	if (l->line[l->line_iter] == ' ' || l->line[l->line_iter] == '\t')
		return (handle_reading_whitespace(l, ctx));
	else if (l->line[l->line_iter] == '$')
		return (handle_variable(l, ctx));
	else if (l->line[l->line_iter] == '1')
		return (handle_1(l));
	else if (l->line[l->line_iter] == '2')
		return (handle_2(l, ctx));
	else if (l->line[l->line_iter] == '\'')
		return (handle_symbol(l, IN_SINGLE_QUOTE, ctx));
	else if (l->line[l->line_iter] == '"')
		return (handle_symbol(l, IN_DOUBLE_QUOTE, ctx));
	else if (l->line[l->line_iter] == '>')
		return (handle_symbol(l, CHECK_APPEND, ctx));
	else if (l->line[l->line_iter] == '<')
		return (handle_symbol(l, CHECK_HERE_DOC, ctx));
	else if (l->line[l->line_iter] == '|' || l->line[l->line_iter] == '&')
		return (handle_symbol(l, END, ctx));
	else if (l->line[l->line_iter] == '(')
		return (handle_symbol(l, END, ctx));
	else if (l->here_eof == true && l->first_blank == false)
		l->first_blank = true;
	l->buffer[l->buf_index++] = l->line[l->line_iter];
	return (INITIAL);
}

t_state	handle_in_single_quote(t_lexer *lexer)
{
	end_of_heredoc_check(lexer);
	if (lexer->line[lexer->line_iter] == '\'')
	{
		if (lexer->buf_index == 0)
			create_token(T_WORD, "\0", lexer);
		else
			flush_buffer(lexer, T_WORD);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
	return (IN_SINGLE_QUOTE);
}

t_state	handle_in_double_quote(t_lexer *lexer, t_ctx *ctx)
{
	if (lexer->here_eof == true)
	{
		lexer->tok.t[lexer->heredoc_index]->hereexpand = true;
		end_of_heredoc_check(lexer);
	}
	if (lexer->line[lexer->line_iter] == '$')
	{
		lexer->curent_string = '"';
		return (handle_variable(lexer, ctx));
	}
	if (lexer->line[lexer->line_iter] == '"')
	{
		if (lexer->buf_index == 0)
			create_token(T_WORD, "\0", lexer);
		else
			flush_buffer(lexer, T_WORD);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
	return (IN_DOUBLE_QUOTE);
}

t_state	handle_check_append(t_lexer *lexer, int i, t_ctx *ctx)
{
	end_of_heredoc_check(lexer);
	if (lexer->line[lexer->line_iter] == '>')
	{
		if (i == 1)
			lexer->tok.t[lexer->tok.token_iter]
				= create_token(TOKEN_APPEND, ">>", lexer);
		else if (i == 2)
			lexer->tok.t[lexer->tok.token_iter]
				= create_token(TOKEN_APPEND_2, "2>>", lexer);
		return (INITIAL);
	}
	if (i == 1)
		lexer->tok.t[lexer->tok.token_iter]
			= create_token(T_REDIRECT_STDOUT, ">", lexer);
	else if (i == 2)
		lexer->tok.t[lexer->tok.token_iter]
			= create_token(TOKEN_REDIRECT_STDERR, "2>", lexer);
	if (lexer->line[lexer->line_iter])
		return (handle_initial(lexer, ctx));
	else
		return (INITIAL);
}

t_state	handle_check_here_doc(t_lexer *lexer, int i, t_ctx *ctx)
{
	end_of_heredoc_check(lexer);
	if (lexer->line[lexer->line_iter] == '<')
	{
		if (i == 1)
			lexer->tok.t[lexer->tok.token_iter]
				= create_token(TOKEN_HERE_DOC, "<<", lexer);
		else if (i == 2)
			lexer->tok.t[lexer->tok.token_iter]
				= create_token(TOKEN_HERE_DOC_2, "2<<", lexer);
		return (INITIAL);
	}
	if (i == 1)
		lexer->tok.t[lexer->tok.token_iter]
			= create_token(TOKEN_REDIRECT_STDIN, "<", lexer);
	else if (i == 2)
		lexer->tok.t[lexer->tok.token_iter]
			= create_token(TOKEN_REDIRECT_IN_2, "2<", lexer);
	if (lexer->line[lexer->line_iter])
		return (handle_initial(lexer, ctx));
	else
		return (INITIAL);
}
