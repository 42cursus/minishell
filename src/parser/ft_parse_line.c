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
	int			ec;

	ec = EX_OK;
	scan_the_line(line + ft_strspn(line, " \t"), &lexer, ctx);
	if (!lexer.err)
	{
		if (lexer.tok.t[lexer.tok.token_iter]->type == TOKEN_BLANK)
			skip_blanks(lexer.tok.t, &lexer.tok.token_iter, NULL, &lexer);
		*root = pipeline_loop(&lexer, ctx);
		ctx->hd.size = ctx->hd.ss;
		ctx->hd.ss = 0;
		if (ft_sh_env_map_get_entry("DEBUG", ctx))
			print_tokens(&lexer);
	}
	if (lexer.err != 0)
		ec = handle_parser_err(lexer.err, &lexer);
	free_tokens(&lexer);
	if (ec == 1 || ec == 3 || ec == 7 || ec == 8 || ec == 10)
		ec = EX_BADSYNTAX;
	else if (ec != 0)
		ec = EX_MISCERROR;
	ctx->last_status_code = ec;
	return (ec);
}
