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

t_state	create_pid_token(t_lexer *lexer, t_ctx *ctx)
{
	char	*pid;

	pid = ft_itoa(ft_getpid_c());
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
	else if (state == CHECK_APPEND || state == CHECK_HERE_DOC)
		lexer->err = NO_REDIR_TARGET;
	lexer->tok.t[lexer->tok.token_iter] = NULL;
	lexer->tok.tokens_size = lexer->tok.token_iter;
	lexer->tok.token_iter = 0;
	if (lexer->tok.tokens_size == 0)
		lexer->err = EMPTY_LINE;
	return (lexer->err);
}
