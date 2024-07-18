/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 15:15:08 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 15:15:08 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

/**
 * http://www.oilshell.org/blog/2016/11/01.html
 * https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
 * https://eli.thegreenplace.net
 * 	/2010/01/02/top-down-operator-precedence-parsing
 * https://eli.thegreenplace.net
 * 	/2012/08/02/parsing-expressions-by-precedence-climbing
 * https://eli.thegreenplace.net
 * 	/2009/03/20/a-recursive-descent-parser-with-an-infix-expression-evaluator
 */
int	ft_sh_split_line(char *input, t_ctx *ctx)
{
	char	*p = input;
	int		arg_count;
	char	*start = NULL;
	int		in_single_quotes = 0;
	int		in_double_quotes = 0;

	arg_count = 0;
	if (ctx->argv)
		ctx->argv = (free(ctx->argv), NULL);
	ctx->argv = (char **)malloc(ARG_MAX);
	if (!ctx->argv)
		return (-1);
	while (*p)
	{
		while (ft_isspace(*p)) // Skip leading spaces
			p++;
		start = p;
		while (*p && (!ft_isspace(*p) || in_single_quotes || in_double_quotes))
		{
			if (*p == '\'' && !in_double_quotes)
				in_single_quotes = !in_single_quotes;
			else if (*p == '"' && !in_single_quotes)
				in_double_quotes = !in_double_quotes;
			p++;
		}
		if (p > start)
			ctx->argv[arg_count++] = ft_strndup(start, p - start);
		if (*p)
		{
			*p = '\0'; // Null terminate the token
			p++; // Move past the delimiter
		}
	}
	ctx->argv[arg_count] = NULL;
	ctx->argc = arg_count;
	return (0);
}
