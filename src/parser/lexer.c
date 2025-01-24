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
	{ .token_id = T_WORD,				.str = "TOKEN_WORD" },
	{ .token_id = TOKEN_PIPE,			.str = "TOKEN_PIPE" },
	{ .token_id = T_REDIRECT_STDOUT,	.str = "TOKEN_REDIRECT_OUT" },
	{ .token_id = TOKEN_REDIRECT_STDIN,	.str = "TOKEN_REDIRECT_IN" },
	{ .token_id = TOKEN_HERE_DOC,		.str = "TOKEN_HERE_DOC" },
	{ .token_id = TOKEN_APPEND,			.str = "TOKEN_APPEND" },
	{ .token_id = TOKEN_STRING,			.str = "TOKEN_STRING" },
	{ .token_id = TOKEN_BLANK,			.str = "TOKEN_BLANK" },
	{ .token_id = T_VAR,				.str = "TOKEN_VAR" },
	{ .token_id = TOKEN_END,			.str = "TOKEN_END" },
	{ .token_id = TOKEN_REDIRECT_STDERR,		.str = "TOKEN_REDIRECT_OUT_2" },
	{ .token_id = TOKEN_REDIRECT_IN_2,			.str = "TOKEN_REDIRECT_IN_2" },
	{ .token_id = TOKEN_HERE_DOC_2,				.str = "TOKEN_HERE_DOC_2" },
	{ .token_id = TOKEN_APPEND_2,				.str = "TOKEN_APPEND_2" }
};

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

void	print_tokens(t_lexer *lexer)
{
	printf("\n\nTokens:\n");
	lexer->line_iter = -1;
	while (lexer->tok.t[++(lexer->line_iter)] != NULL)
	{
		if (!lexer->tok.t[lexer->line_iter]->value)
		{
			printf("Token Type: \"%s\", Value: %s\n",
			   get_idstring(lexer->tok.t[lexer->line_iter]->type), "NULL");
		}
		else if (lexer->tok.t[lexer->line_iter]->value && ft_strlen(lexer->tok.t[lexer->line_iter]->value) == 0)
		{
			printf("Token Type: \"%s\", Value: %s\n",
			   get_idstring(lexer->tok.t[lexer->line_iter]->type), "(empty string)");
		}
		else
			printf("Token Type: \"%s\", Value: %s\n",
			   get_idstring(lexer->tok.t[lexer->line_iter]->type), lexer->tok.t[lexer->line_iter]->value);
	}
}

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

t_state	handle_symbol(t_lexer *lexer, t_state state)
{
	flush_buffer(lexer, T_WORD);
	if (state == INITIAL)
	{
		end_of_heredoc_check(lexer);
		lexer->tok.t[lexer->tok.token_iter] = create_token(TOKEN_PIPE, "|", lexer);
	}
	return (state);
}

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
		return (handle_symbol(l, IN_SINGLE_QUOTE));
	else if (l->line[l->line_iter] == '"')
		return (handle_symbol(l, IN_DOUBLE_QUOTE));
	else if (l->line[l->line_iter] == '>')
		return (handle_symbol(l, CHECK_APPEND));
	else if (l->line[l->line_iter] == '<')
		return (handle_symbol(l, CHECK_HERE_DOC));
	else if (l->line[l->line_iter] == '|')
		return (handle_symbol(l, INITIAL));
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
	return (handle_initial(lexer, ctx));
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
	return (handle_initial(lexer, ctx));
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

t_state	exit_variable(t_lexer *l, t_ctx *ctx)
{
	char	*sc;

	if (l->line[(l->line_iter)++] == '?' && l->buf_index == 0)
	{
		sc = ft_itoa(ctx->status_code);
		create_token(T_WORD, sc, l);
		free(sc);
	}
	if (l->buf_index > 0)
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

t_state	create_pid_token(t_lexer *lexer, t_ctx *ctx)
{
	char	*pid;

	pid = ft_itoa(ft_getpid());
	if (!pid)
	{
		lexer->err = PID_ALLOC_FAILURE;
		return (END);
	}
	create_token(T_WORD, pid, lexer);
	free(pid);
	lexer->line_iter++;
	return (exit_variable(lexer, ctx));
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
		else if (lexer->buf_index == 0 && !ft_isalpha(c) && c != '_')
			return (exit_variable(lexer, ctx));
		else if ((lexer->buf_index > 0) && (ft_isalnum(c) || c == '_'))
			lexer->buffer[lexer->buf_index++] = c;
		else
			break ;
	}
	return (exit_variable(lexer, ctx));
}

t_state	scan_loop(t_lexer *l, t_ctx *ctx)
{
	t_state	current_state;

	current_state = INITIAL;
	while (l->line[(l->line_iter)] != '\0' && l->err == 0)
	{
		if (current_state == INITIAL)
			current_state = handle_initial(l, ctx);
		else if (current_state == IN_SINGLE_QUOTE)
			current_state = handle_in_single_quote(l);
		else if (current_state == IN_DOUBLE_QUOTE)
			current_state = handle_in_double_quote(l, ctx);
		else if (current_state == CHECK_APPEND)
			current_state = handle_check_append(l, 1, ctx);
		else if (current_state == CHECK_HERE_DOC)
			current_state = handle_check_here_doc(l, 1, ctx);
		if (l->line[(l->line_iter)] != '\0')
			(l->line_iter)++;
	}
	return (current_state);
}

int	scan_the_line(const char *line, t_lexer *lexer, t_ctx *ctx)
{
	t_state	state;

	*lexer = (t_lexer){
		.curent_string = '0',
		.line = (char *)line,
	};
	state = scan_loop(lexer, ctx);
	if (lexer->buf_index != 0 && state == INITIAL)
		flush_buffer(lexer, T_WORD);
	else if (state == IN_DOUBLE_QUOTE || state == IN_SINGLE_QUOTE)
		lexer->err = UNCLOSED_QUOTE;
	lexer->tok.t[lexer->tok.token_iter] = NULL;
	print_tokens(lexer);
	lexer->tok.tokens_size = lexer->tok.token_iter;
	lexer->tok.token_iter = 0;
	return (lexer->err);
}
