/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_give_terminal_to.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:11:43 by abelov            #+#    #+#             */
/*   Updated: 2025/02/02 18:11:44 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Give the terminal to PGRP.  */
int	ft_give_terminal_to(pid_t pgrp)
{
	sigset_t	set;
	sigset_t	oset;

	sigemptyset(&set);
	sigaddset(&set, SIGTTOU);
	sigaddset(&set, SIGTTIN);
	sigaddset(&set, SIGTSTP);
	sigaddset(&set, SIGCHLD);
	sigemptyset(&oset);
	ft_sigprocmask(SIG_BLOCK, &set, &oset);

	if (ft_tcsetpgrp(SHELL_TTY_FILENO, pgrp) < 0)
	{
		ft_dprintf(STDERR_FILENO,
				   "ft_tcsetpgrp(%d) failed: pid %ld to pgrp %ld",
				   SHELL_TTY_FILENO, (long) ft_getpid(), pgrp);
		return (-1);
	}
	ft_sigprocmask(SIG_SETMASK, &oset, (sigset_t *) NULL);
	return (0);
}
