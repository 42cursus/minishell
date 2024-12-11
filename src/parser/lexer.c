/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <sbelcher@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:20:53 by sbelcher          #+#    #+#             */
/*   Updated: 2024/11/15 15:20:54 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct
{
	int token_id;
	const char *str;
} lt_entry;

lt_entry lt[12] = {
	{ .token_id = TOKEN_COMMAND,		.str = "TOKEN_COMMAND" },
	{ .token_id = TOKEN_WORD,			.str = "TOKEN_WORD" },
	{ .token_id = TOKEN_PIPE,			.str = "TOKEN_PIPE" },
	{ .token_id = TOKEN_REDIRECT_OUT,	.str = "TOKEN_REDIRECT_OUT" },
	{ .token_id = TOKEN_REDIRECT_IN,	.str = "TOKEN_REDIRECT_IN" },
	{ .token_id = TOKEN_HERE_DOC,		.str = "TOKEN_HERE_DOC" },
	{ .token_id = TOKEN_APPEND,			.str = "TOKEN_APPEND" },
	{ .token_id = TOKEN_STRING,			.str = "TOKEN_STRING" },
	{ .token_id = TOKEN_BLANK,			.str = "TOKEN_BLANK" },
	{ .token_id = TOKEN_VAR,			.str = "TOKEN_VAR" },
	{ .token_id = TOKEN_END,			.str = "TOKEN_END" }
};

t_token *create_token(t_token_type type, const char *value, t_lexer *lexer)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token) return NULL;
	token->type = type;
	token->value = strdup(value);
	lexer->token_count++;
	return (token);
}

void flush_buffer(t_lexer *lexer, t_token_type type)
{
	if (lexer->buf_index > 0)
	{
		lexer->buffer[lexer->buf_index] = '\0';
		lexer->tokens[lexer->token_count] = create_token(type, lexer->buffer, lexer);
		lexer->buf_index = 0;
	}
}

void	free_tokens(t_lexer *lexer)
{
	int	i;

	i = -1;
	while (++i < lexer->token_count)
	{
		free(lexer->tokens[i]->value);
		free(lexer->tokens[i]);
		lexer->token_count = 0;
	}
}

t_state	handle_initial(char c, t_lexer *lexer)
{
	if (isspace(c))
	{
		lexer->tokens[lexer->token_count] = create_token(TOKEN_BLANK, " ", lexer);
	  return (READING_WHITESPACE);
	}
	else if (c == '$')
	{
		lexer->buffer[lexer->buf_index++] = c;
		return (VARIABLE);
	}
	else if (c == '\'')
	  return (IN_SINGLE_QUOTE);
	else if (c == '"')
	  return (IN_DOUBLE_QUOTE);
	else if (c == '>')
	  return (CHECK_APPEND);
	else if (c == '<')
	  return (CHECK_HERE_DOC);
	else if (c == '|')
	{
	  lexer->tokens[lexer->token_count] = create_token(TOKEN_PIPE, "|", lexer);
	  return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = c;
	return (READING_WORD);
}

t_state	handle_reading_word(char c, t_lexer *lexer)
{
	if (c == '$')
	{
		flush_buffer(lexer, TOKEN_WORD);
		lexer->buffer[lexer->buf_index++] = c;
		return (VARIABLE);
	}
	else if (isspace(c))
	{
		flush_buffer(lexer, TOKEN_WORD);
		lexer->tokens[lexer->token_count] = create_token(TOKEN_BLANK, " ", lexer);
		return (READING_WHITESPACE);
	}
	else if (c == '\'')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (IN_SINGLE_QUOTE);
	}
	else if (c == '"')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (IN_DOUBLE_QUOTE);
	}
	else if (c == '>')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (CHECK_APPEND);
	}
	else if (c == '<')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (CHECK_HERE_DOC);
	}
	else if (c == '|')
	{
		flush_buffer(lexer, TOKEN_WORD);
		lexer->tokens[lexer->token_count] = create_token(TOKEN_PIPE, "|", lexer);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = c;
	return (READING_WORD);
}

t_state	handle_in_single_quote(char c, t_lexer *lexer)
{
	if (c == '\'')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = c;
	return (IN_SINGLE_QUOTE);
}

t_state	handle_in_double_quote(char c, t_lexer *lexer)
{
	if (c == '$')
	{
		lexer->curent_string = '"';
		flush_buffer(lexer, TOKEN_WORD);
		lexer->buffer[lexer->buf_index++] = c;
		return (VARIABLE);
	}
	if (c == '"')
	{
		
		flush_buffer(lexer, TOKEN_WORD);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = c;
	return (IN_DOUBLE_QUOTE);
}

t_state	handle_check_append(char c, t_lexer *lexer)
{
	if (c == '>')
	{
		lexer->tokens[lexer->token_count] = create_token(TOKEN_APPEND, ">>", lexer);
		return (INITIAL);
	}
	lexer->tokens[lexer->token_count] = create_token(TOKEN_REDIRECT_OUT, ">", lexer);
	return (handle_initial(c, lexer));
}

t_state	handle_check_here_doc(char c, t_lexer *lexer)
{
	if (c == '<')
	{
		lexer->tokens[lexer->token_count] = create_token(TOKEN_HERE_DOC, "<<", lexer);
		return (INITIAL);
	}
	lexer->tokens[lexer->token_count] = create_token(TOKEN_REDIRECT_IN, "<", lexer);
	return (handle_initial(c, lexer));
}

t_state	handle_reading_whitespace(char c, t_lexer *lexer)
{
	if (!isspace(c))
		return (handle_initial(c, lexer));
	else
		return READING_WHITESPACE;
}

t_state	handle_variable(char c, t_lexer *lexer)
{
	if (lexer->buf_index == 1 && (ft_isalpha(c) || c == '_'))
	{
		lexer->buffer[lexer->buf_index++] = c;
		return (VARIABLE);
	}
	else if (lexer->buf_index == 1 && !ft_isalpha(c) && c != '_')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (handle_initial(c, lexer));
	}
	else if ((lexer->buf_index > 1) && (ft_isalnum(c) || c == '_'))
	{
		lexer->buffer[lexer->buf_index++] = c;
		return (VARIABLE);
	}
	else
	{
		flush_buffer(lexer, TOKEN_VAR);
		if (lexer->curent_string == '"')
		{
			lexer->curent_string = '0';
			if (c == '"')
				return (INITIAL);
			return (handle_in_double_quote(c, lexer));
		}
		return (handle_initial(c, lexer));
	}
}

const char *get_idstring(int token)
{
	int i;
	const char *str = NULL;

	i = -1;
	while (++i < 11)
	{
		if (lt[i].token_id == token)
		{
			str = lt[i].str;
			break ;
		}
	}
	return str;
}

t_lexer scan_the_Line(const char *line)
{
	int i;
	t_state current_state = INITIAL;
	t_lexer lexer = { .buf_index = 0, .token_count = 0 };
	lexer.curent_string = '0';
	i = -1;
	while (line[++i] != '\0')
	{
		if (current_state == INITIAL)
			current_state = handle_initial(line[i], &lexer);
		else if (current_state == READING_WORD)
			current_state = handle_reading_word(line[i], &lexer);
		else if (current_state == IN_SINGLE_QUOTE)
			current_state = handle_in_single_quote(line[i], &lexer);
		else if (current_state == IN_DOUBLE_QUOTE)
			current_state = handle_in_double_quote(line[i], &lexer);
		else if (current_state == CHECK_APPEND)
			current_state = handle_check_append(line[i], &lexer);
		else if (current_state == CHECK_HERE_DOC)
			current_state = handle_check_here_doc(line[i], &lexer);
		else if (current_state == READING_WHITESPACE)
			current_state = handle_reading_whitespace(line[i], &lexer);
		else if (current_state == VARIABLE)
			current_state = handle_variable(line[i], &lexer);
	}
	if (current_state == VARIABLE)
		flush_buffer(&lexer, TOKEN_VAR);
	else if (lexer.buf_index != 0)
		flush_buffer(&lexer, TOKEN_WORD);
	flush_buffer(&lexer, TOKEN_WORD);
	ft_printf("\nTokens:\n");
	i = -1;
	while (lexer.tokens[++i] != NULL)
		printf("Token Type: \"%s\", Value: %s\n",
	get_idstring(lexer.tokens[i]->type), lexer.tokens[i]->value);
	return lexer;
}
