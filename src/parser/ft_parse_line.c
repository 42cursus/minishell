/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:10:01 by abelov            #+#    #+#             */
/*   Updated: 2025/01/21 17:10:02 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_do_parse(const char *line, t_ast_node **root, t_ctx *ctx)
{
	t_lexer		lexer;
	int			errcode;

	errcode = 0;
	errcode = scan_the_line(line, &lexer, ctx);
	if (errcode != 0)
		errcode = handle_parser_err(errcode, &lexer);
	if (errcode == 0)
	{
		if (lexer.tok.t[lexer.tok.token_iter]->type == TOKEN_BLANK)
			skip_blanks(lexer.tok.t, &lexer.tok.token_iter, NULL, &lexer);
		*root = pipeline_loop(&lexer, ctx);
		ctx->hd.size = ctx->hd.ss;
		ctx->hd.ss = 0;
		errcode = lexer.err;
		if (errcode != 0)
			errcode = handle_parser_err(errcode, &lexer);
	}
	free_tokens(&lexer);
	return (errcode);
}
