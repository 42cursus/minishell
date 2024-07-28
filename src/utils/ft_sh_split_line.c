/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_split_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 15:15:08 by abelov            #+#    #+#             */
/*   Updated: 2024/07/21 00:06:09 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_sh_switch_par_state(const char *p, int *in_single_quotes,
							int *in_double_quotes)
{
	if (*p == '\'' && !(*in_double_quotes))
		(*in_single_quotes) = !(*in_single_quotes);
	else if (*p == '"' && !(*in_single_quotes))
		(*in_double_quotes) = !(*in_double_quotes);
}

int	ft_sh_do_split(char *p, t_ctx *ctx)
{
	char	*start;
	int		in_single_quotes;
	int		in_double_quotes;
	int		arg_count;

	arg_count = 0;
	start = NULL;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*p)
	{
		while (ft_isspace(*p)) // Skip leading spaces
			p++;
		start = p;
		while (*p && (!ft_isspace(*p) || in_single_quotes || in_double_quotes))
			ft_sh_switch_par_state(p++, &in_single_quotes, &in_double_quotes);
		if (p > start)
			ctx->argv[arg_count++] = ft_strndup(start, p - start);
		if (*p)
		{
			*p = '\0'; // Null terminate the token
			p++; //Move past the delimiter
		}
	}
	return (arg_count);
}

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
	int	arg_count;

	if (ctx->argv)
	{
		while (ctx->argc--)
			free(ctx->argv[ctx->argc]);
		ctx->argv = (free(ctx->argv), NULL);
	}
	ctx->argv = (char **)malloc(ARG_MAX);
	if (!ctx->argv)
		return (free(input), -1);
	arg_count = ft_sh_do_split(input, ctx);
	ctx->argv[arg_count] = NULL;
	ctx->argc = arg_count;
	free(input);
	return (0);
}
