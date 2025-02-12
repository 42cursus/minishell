/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_init_term.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:56:20 by abelov            #+#    #+#             */
/*   Updated: 2025/02/12 16:56:57 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_sh_init_term(t_ctx *ctx)
{
	const pid_t	mypid = ft_getpid();
	int			retaval;

	retaval = 0;
	if (mypid != ft_getpgrp() && ft_setpgid(0, mypid) < 0)
		ft_perrorf("%s: ft_setpgid (%d to %d)", ctx->cmd_name, mypid, 0);
	ctx->parent_tpgrp = ft_tcgetpgrp(STDIN_FILENO);
	dup2(STDIN_FILENO, SHELL_TTY_FILENO);
	if (ctx->parent_tpgrp == (-1))
	{
		ft_dprintf(STDERR_FILENO, "(no job control in this shell)\n");
		retaval = -1;
	}
	else if (mypid != ft_tcgetpgrp(SHELL_TTY_FILENO)
		&& ft_sh_give_terminal_to(mypid) < 0)
	{
		ft_dprintf(STDERR_FILENO,
			"(cannot set terminal process group (%d to %d) %d)\n",
			mypid, ft_tcgetpgrp(SHELL_TTY_FILENO), errno);
		retaval = -1;
	}
	return (retaval);
}
