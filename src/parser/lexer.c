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

lt_entry lt[] = {
	{ .token_id = TOKEN_COMMAND,		.str = "TOKEN_COMMAND" },
	{ .token_id = TOKEN_WORD,			.str = "TOKEN_WORD" },
	{ .token_id = TOKEN_PIPE,			.str = "TOKEN_PIPE" },
	{ .token_id = TOKEN_REDIRECT_STDOUT,	.str = "TOKEN_REDIRECT_OUT" },
	{ .token_id = TOKEN_REDIRECT_STDIN,	.str = "TOKEN_REDIRECT_IN" },
	{ .token_id = TOKEN_HERE_DOC,		.str = "TOKEN_HERE_DOC" },
	{ .token_id = TOKEN_APPEND,			.str = "TOKEN_APPEND" },
	{ .token_id = TOKEN_STRING,			.str = "TOKEN_STRING" },
	{ .token_id = TOKEN_BLANK,			.str = "TOKEN_BLANK" },
	{ .token_id = TOKEN_VAR,			.str = "TOKEN_VAR" },
	{ .token_id = TOKEN_END,			.str = "TOKEN_END" },
	{ .token_id = TOKEN_REDIRECT_STDERR,			.str = "TOKEN_REDIRECT_OUT_2" },
	{ .token_id = TOKEN_REDIRECT_IN_2,			.str = "TOKEN_REDIRECT_IN_2" },
	{ .token_id = TOKEN_HERE_DOC_2,				.str = "TOKEN_HERE_DOC_2" },
	{ .token_id = TOKEN_APPEND_2,				.str = "TOKEN_APPEND_2" }
};

t_token *create_token(t_token_type type, const char *value, t_lexer *lexer)
{
	t_token *token = (t_token *)malloc(sizeof(t_token));
	if (!token) return NULL;
	token->type = type;
	token->value = strdup(value);

	lexer->tokens[lexer->token_iter++] = token;
	return (token);
}

void flush_buffer(t_lexer *lexer, t_token_type type)
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
	while (lexer->tokens[++i] != NULL)
	{
		free(lexer->tokens[i]->value);
		free(lexer->tokens[i]);
		lexer->token_iter = 0;
	}
}

t_state	handle_initial(t_lexer *lexer)
{
	if (isspace(lexer->line[lexer->line_iter]))
	{
		lexer->tokens[lexer->token_iter] = create_token(TOKEN_BLANK, " ", lexer);
		return (READING_WHITESPACE);
	}
	else if (lexer->line[lexer->line_iter] == '$')
		return (VARIABLE);
	else if (lexer->line[lexer->line_iter] == '1')
	{
		lexer->line_iter++;
		if (lexer->line[lexer->line_iter] == '<')
			return (CHECK_HERE_DOC);
		else if (lexer->line[lexer->line_iter] == '>')
			return (CHECK_APPEND);
		else
			lexer->line_iter--;
		lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
		return (READING_WORD);
	}
	else if (lexer->line[lexer->line_iter] == '2')
	{
		lexer->line_iter++;
		if (lexer->line[lexer->line_iter] == '<')
		{
			lexer->line_iter++;
			if (lexer->line[lexer->line_iter] == '<')
			{
				lexer->tokens[lexer->token_iter] = create_token(TOKEN_HERE_DOC_2, "2<<", lexer);
				return (INITIAL);
			}
			lexer->tokens[lexer->token_iter] = create_token(TOKEN_REDIRECT_IN_2, "2<", lexer);
			return (handle_initial(lexer));
		}
		else if (lexer->line[lexer->line_iter] == '>')
		{
			lexer->line_iter++;
			if (lexer->line[lexer->line_iter] == '>')
			{
				lexer->tokens[lexer->token_iter] = create_token(TOKEN_APPEND_2, "2>>", lexer);
				return (INITIAL);
			}
			lexer->tokens[lexer->token_iter] = create_token(TOKEN_REDIRECT_STDERR, "2>", lexer);
			return (handle_initial(lexer));
		}
		else
			lexer->line_iter--;
		lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
		return (READING_WORD);
	}
	else if (lexer->line[lexer->line_iter] == '\'')
		return (IN_SINGLE_QUOTE);
	else if (lexer->line[lexer->line_iter] == '"')
		return (IN_DOUBLE_QUOTE);
	else if (lexer->line[lexer->line_iter] == '>')
		return (CHECK_APPEND);
	else if (lexer->line[lexer->line_iter] == '<')
		return (CHECK_HERE_DOC);
	else if (lexer->line[lexer->line_iter] == '|')
	{
		lexer->tokens[lexer->token_iter] = create_token(TOKEN_PIPE, "|", lexer);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
	return (READING_WORD);
}

t_state	handle_reading_word(t_lexer *lexer)
{
	if (lexer->line[lexer->line_iter] == '$')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (VARIABLE);
	}
	if (lexer->line[lexer->line_iter] == '1'|| lexer->line[lexer->line_iter] == '2')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (handle_initial(lexer));
	}
	else if (isspace(lexer->line[lexer->line_iter]))
	{
		flush_buffer(lexer, TOKEN_WORD);
		lexer->tokens[lexer->token_iter] = create_token(TOKEN_BLANK, " ", lexer);
		return (READING_WHITESPACE);
	}
	else if (lexer->line[lexer->line_iter] == '\'')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (IN_SINGLE_QUOTE);
	}
	else if (lexer->line[lexer->line_iter] == '"')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (IN_DOUBLE_QUOTE);
	}
	else if (lexer->line[lexer->line_iter] == '>')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (CHECK_APPEND);
	}
	else if (lexer->line[lexer->line_iter] == '<')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (CHECK_HERE_DOC);
	}
	else if (lexer->line[lexer->line_iter] == '|')
	{
		flush_buffer(lexer, TOKEN_WORD);
		lexer->tokens[lexer->token_iter] = create_token(TOKEN_PIPE, "|", lexer);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
	return (READING_WORD);
}

t_state	handle_in_single_quote(t_lexer *lexer)
{
	if (lexer->line[lexer->line_iter] == '\'')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
	return (IN_SINGLE_QUOTE);
}

t_state	handle_in_double_quote(t_lexer *lexer)
{
	if (lexer->line[lexer->line_iter] == '$')
	{
		lexer->curent_string = '"';
		flush_buffer(lexer, TOKEN_WORD);
		return (VARIABLE);
	}
	if (lexer->line[lexer->line_iter] == '"')
	{

		flush_buffer(lexer, TOKEN_WORD);
		return (INITIAL);
	}
	lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
	return (IN_DOUBLE_QUOTE);
}

t_state	handle_check_append(t_lexer *lexer)
{
	if (lexer->line[lexer->line_iter] == '>')
	{
		lexer->tokens[lexer->token_iter] = create_token(TOKEN_APPEND, ">>", lexer);
		return (INITIAL);
	}
	lexer->tokens[lexer->token_iter] = create_token(TOKEN_REDIRECT_STDOUT, ">", lexer);
	return (handle_initial(lexer));
}

t_state	handle_check_here_doc(t_lexer *lexer)
{
	if (lexer->line[lexer->line_iter] == '<')
	{
		lexer->tokens[lexer->token_iter] = create_token(TOKEN_HERE_DOC, "<<", lexer);
		return (INITIAL);
	}
	lexer->tokens[lexer->token_iter] = create_token(TOKEN_REDIRECT_STDIN, "<", lexer);
	return (handle_initial(lexer));
}

t_state	handle_reading_whitespace(t_lexer *lexer)
{
	if (!(lexer->line[lexer->line_iter] == ' ') && !(lexer->line[lexer->line_iter] == '\t'))
		return (handle_initial(lexer));
	else
		return READING_WHITESPACE;
}

t_state	handle_variable(t_lexer *lexer)
{
	if (lexer->buf_index == 0 && lexer->line[lexer->line_iter] == '?')
	{
		lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
		flush_buffer(lexer, TOKEN_VAR);
		if (lexer->curent_string == '"')
			return (IN_DOUBLE_QUOTE);
		return (INITIAL);
	}
	if (lexer->buf_index == 0 && (ft_isalpha(lexer->line[lexer->line_iter]) || lexer->line[lexer->line_iter] == '_'))
	{
		lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
		return (VARIABLE);
	}
	else if (lexer->buf_index == 0 && !ft_isalpha(lexer->line[lexer->line_iter]) && lexer->line[lexer->line_iter] != '_')
	{
		flush_buffer(lexer, TOKEN_WORD);
		return (handle_initial(lexer));
	}
	else if ((lexer->buf_index > 0) && (ft_isalnum(lexer->line[lexer->line_iter]) || lexer->line[lexer->line_iter] == '_'))
	{
		lexer->buffer[lexer->buf_index++] = lexer->line[lexer->line_iter];
		return (VARIABLE);
	}
	else
	{
		flush_buffer(lexer, TOKEN_VAR);
		if (lexer->curent_string == '"')
		{
			lexer->curent_string = '0';
			if (lexer->line[lexer->line_iter] == '"')
				return (INITIAL);
			return (handle_in_double_quote(lexer));
		}
		return (handle_initial(lexer));
	}
}

const char *get_idstring(int token)
{
	int i;
	const char *str = NULL;

	i = -1;
	while (++i < TOKEN_MAX)
	{
		if (lt[i].token_id == token)
		{
			str = lt[i].str;
			break ;
		}
	}
	return str;
}

int scan_the_Line(const char *line, t_lexer *lexer)
{
	t_state current_state = INITIAL;
	*lexer = (t_lexer){
		.curent_string = '0',
		.line = (char *)line,
		.line_iter = -1
	};

	current_state = INITIAL;
	while (lexer->line[++(lexer->line_iter)] != '\0')
	{
		if (current_state == INITIAL)
			current_state = handle_initial(lexer);
		else if (current_state == READING_WORD)
			current_state = handle_reading_word(lexer);
		else if (current_state == IN_SINGLE_QUOTE)
			current_state = handle_in_single_quote(lexer);
		else if (current_state == IN_DOUBLE_QUOTE)
			current_state = handle_in_double_quote(lexer);
		else if (current_state == CHECK_APPEND)
			current_state = handle_check_append(lexer);
		else if (current_state == CHECK_HERE_DOC)
			current_state = handle_check_here_doc(lexer);
		else if (current_state == READING_WHITESPACE)
			current_state = handle_reading_whitespace(lexer);
		else if (current_state == VARIABLE)
			current_state = handle_variable(lexer);
	}
	if (current_state == VARIABLE)
		flush_buffer(lexer, TOKEN_VAR);
	else if (lexer->buf_index != 0)
		flush_buffer(lexer, TOKEN_WORD);
	lexer->tokens[lexer->token_iter] = NULL;
	printf("\n\nTokens:\n");
	lexer->line_iter = -1;
	while (lexer->tokens[++(lexer->line_iter)] != NULL)
		printf("Token Type: \"%s\", Value: %s\n",
			   get_idstring(lexer->tokens[lexer->line_iter]->type), lexer->tokens[lexer->line_iter]->value);

	lexer->tokens_size = lexer->token_iter;
	lexer->token_iter = 0;
	return (0);
}
