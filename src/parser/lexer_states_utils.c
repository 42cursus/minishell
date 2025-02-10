/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_states_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:29:27 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/10 16:29:30 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_state	handle_1(t_lexer *lexer)
{
	flush_buffer(lexer, T_WORD);
	lexer->line_iter++;
	if (lexer->line[lexer->line_iter] == '<')
		return (CHECK_HERE_DOC);
	else if (lexer->line[lexer->line_iter] == '>')
		return (CHECK_APPEND);
	else
		lexer->line_iter--;
	lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
	return (INITIAL);
}

t_state	handle_2(t_lexer *lexer, t_ctx *ctx)
{
	flush_buffer(lexer, T_WORD);
	if (lexer->line[++lexer->line_iter] == '<')
	{
		lexer->line_iter++;
		return (handle_check_here_doc(lexer, 2, ctx));
	}
	else if (lexer->line[lexer->line_iter] == '>')
	{
		lexer->line_iter++;
		return (handle_check_append(lexer, 2, ctx));
	}
	else
		lexer->buffer[lexer->buf_index++] = lexer->line[--lexer->line_iter];
	return (INITIAL);
}

t_state	handle_symbol(t_lexer *l, t_state state, t_ctx *ctx)
{
	char	c;

	flush_buffer(l, T_WORD);
	if (state != END)
		return (state);
	end_of_heredoc_check(l);
	c = l->line[l->line_iter];
	if ((c == '|' || c == '&') && l->line_iter == 0)
		l->err = OP_OUT_OF_PLACE;
	if (l->line[++l->line_iter] == '\0')
		l->err = OP_OUT_OF_PLACE;
	else if (l->line[l->line_iter] == '|' && c == '|')
		l->tok.t[l->tok.token_iter] = create_token(TOKEN_OR, "||", l);
	else if (l->line[l->line_iter] != '|' && c == '|')
	{
		l->tok.t[l->tok.token_iter] = create_token(TOKEN_PIPE, "|", l);
		return (handle_initial(l, ctx));
	}
	else if (l->line[l->line_iter] == '&' && c == '&')
		l->tok.t[l->tok.token_iter] = create_token(TOKEN_AND, "&&", l);
	else if (l->line[l->line_iter] == ')' && c == '(')
		l->tok.t[l->tok.token_iter] = create_token(TOKEN_DUMMY, "()", l);
	else if (l->line[l->line_iter] != '&' && c == '&')
		l->err = SINGLE_AMPERSAND;
	return (INITIAL);
}

t_state	handle_reading_whitespace(t_lexer *lexer, t_ctx *ctx)
{
	if (lexer->here_eof == true && lexer->first_blank == false)
		lexer->first_blank = true;
	else if (lexer->here_eof == true && lexer->first_blank == true)
		end_of_heredoc_check(lexer);
	flush_buffer(lexer, T_WORD);
	lexer->tok.t[lexer->tok.token_iter] = create_token(TOKEN_BLANK, " ", lexer);
	while ((lexer->line[lexer->line_iter] == ' ')
		|| (lexer->line[lexer->line_iter] == '\t'))
		(lexer->line_iter)++;
	if (lexer->line[lexer->line_iter] != '\0')
		return (handle_initial(lexer, ctx));
	else
		return (INITIAL);
}

t_state	handle_variable(t_lexer *lexer, t_ctx *ctx)
{
	char	c;

	end_of_heredoc_check(lexer);
	flush_buffer(lexer, T_WORD);
	while (lexer->line[++(lexer->line_iter)] != '\0')
	{
		c = lexer->line[(lexer->line_iter)];
		if (lexer->buf_index == 0 && c == '?')
			return (exit_variable(lexer, ctx));
		if (lexer->buf_index == 0 && c == '$')
			return (create_pid_token(lexer, ctx));
		if (lexer->buf_index == 0 && (ft_isalpha(c) || c == '_'))
			lexer->buffer[lexer->buf_index++] = c;
		else if ((lexer->buf_index > 0) && (ft_isalnum(c) || c == '_'))
			lexer->buffer[lexer->buf_index++] = c;
		else
			break ;
	}
	if (lexer->buf_index == 0)
		create_token(T_WORD, "$", lexer);
	return (exit_variable(lexer, ctx));
}
