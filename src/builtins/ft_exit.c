/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:20:36 by abelov            #+#    #+#             */
/*   Updated: 2025/02/10 19:20:37 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_sh_exit_get_val(const t_ctx *ctx)
{
	int			retval;
	long		val;
	char		*endptr;
	const char	*cm = ctx->cmd_name;

	val = ft_strtol(ctx->argv[1], &endptr, 10);
	if (!(errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
		&& !(errno != 0 && val == 0))
		retval = (int) val % 256;
	else
		retval = (ft_dprintf(STDERR_FILENO, "%s: exit: `%s': numeric "
					"argument required\n", cm, ctx->argv[1]), EX_BADUSAGE);
	return (retval);
}

int	ft_exit_with_exitcode(t_ctx *ctx)
{
	int		retval;

	retval = EX_OK;
	ft_printf("exit\n");
	if (ctx->argc > 2)
	{
		errno = E2BIG;
		ft_perrorf("%s: exit", ctx->cmd_name);
		retval = EXIT_FAILURE;
	}
	else if (ctx->argc == 2)
		retval = ft_sh_exit_get_val(ctx);
	else
		retval = ctx->last_status_code;
	ft_cleanup_argv(ctx);
	ft_sh_destroy_ctx(ctx);
	exit(retval);
}
