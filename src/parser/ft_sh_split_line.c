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
 * These are the maximum length and maximum number of strings passed to the
 * execve() system call.  MAX_ARG_STRLEN is essentially random but serves to
 * prevent the kernel from being unduly impacted by misaddressed pointers.
 * MAX_ARG_STRINGS is chosen to fit in a signed 32-bit integer.
 * #include <linux/binfmts.h>
 * #define MAX_ARG_STRLEN (PAGE_SIZE * 32)
 * #define MAX_ARG_STRINGS 0x7FFFFFFF
 *
 * There are also different ways to learn the upper limit
 *      command: getconf ARG_MAX
 *      system call: sysconf(_SC_ARG_MAX)
 *      system header: ARG_MAX in e.g. <[sys/]limits.h>
 *      GNU xargs: xargs --show-limits
 * @see: https://www.in-ulm.de/~mascheck/various/argmax/
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
	return (0);
}
