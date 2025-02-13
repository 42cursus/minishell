/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:02:04 by abelov            #+#    #+#             */
/*   Updated: 2024/07/28 22:02:04 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>

volatile sig_atomic_t	g_received_signal_num = 0;

void	*global_ctx(void *val, t_get_or_set flag)
{
	static void	*ptr = NULL;

	if (flag == SET_VAL)
		ptr = val;
	return (ptr);
}

/**
 * Greeting shell during startup
 */
void	ft_sh_init_welcome(void)
{
	char	*username;

	printf(BANNER);
	username = getenv("USER");
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
}

/**
 * https://brennan.io/2015/01/16/write-a-shell-in-c/
 * https://www.geeksforgeeks.org/making-linux-shell-c/
 */
int	main(int argc, char **argv, char **envp)
{
	int			exitcode;
	t_ctx		*global;

	exitcode = EX_OK;
	if (argc > 1)
		exit(EX_BADUSAGE);
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
	{
		if (ft_sh_init_interactive(&global, envp) == -1)
			exit(EXIT_FAILURE);
		global->argv = argv;
		global->argc = argc;
		global->envp = envp;
		exitcode = ft_sh_loop(global);
		if (ft_sh_give_terminal_to(global->parent_tpgrp))
			ft_perrorf("%s: couldn't allocate terminal", global->cmd_name);
		ft_exit_with_exitcode(global);
	}
	else
		ft_sh_init_noninteractive(&global, envp);
	global = NULL;
	return (exitcode);
}
