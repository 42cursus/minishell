/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 23:04:03 by abelov            #+#    #+#             */
/*   Updated: 2025/01/26 17:47:46 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "minishell.h"

/**
 * Readline C: force return of certain text in readline()
 *
 * https://stackoverflow.com/questions/53165704/
 */
static void	ft_sh_sigint_handler(int sig, siginfo_t *info, void *ctx)
{
	if (sig == SIGINT)
		g_received_signal_num = SIGINT;
	return ;
	(void)sig;
	(void)ctx;
	(void)info;
}

static t_obj_arr *ft_sh_init_builtin_ops(t_obj_arr **ops)
{
	static t_obj_arr	obj;
	static t_shell_op	builtins[] = {
	{.instruction = "pwd", .fun = ft_pwd},
	{.instruction = "env", .fun = ft_env},
	{.instruction = "cd", .fun = ft_chdir},
	{.instruction = "echo", .fun = ft_echo},
	{.instruction = "export", .fun = ft_export},
	{.instruction = "unset", .fun = ft_unset}
	};

	obj.base = builtins;
	obj.elem_size = sizeof(builtins[0]);
	obj.total_elems = sizeof(builtins) / sizeof(builtins[0]);
	obj.cmp_fun = ft_sh_op_cmp;
	ft_qsort_obj(&obj);
	*ops = &obj;
	return (0);
}

static void	ft_sh_set_signal(t_ctx *const ctx)
{
	ctx->act.sa_flags = SA_SIGINFO | SA_RESTART;
	ctx->act.sa_sigaction = &ft_sh_sigint_handler;
	sigemptyset(&ctx->act.sa_mask);
	sigaddset(&ctx->act.sa_mask, SIGINT);

	if (sigaction(SIGINT, &ctx->act, &ctx->inherited_act))
	{
		ft_sh_destroy_ctx(ctx);
		exit(EXIT_FAILURE);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGWINCH, SIG_IGN);
}

int	ft_sh_init_noninteractive(t_ctx **ctx, char **envp)
{
	return (0);
	(void)ctx;
	(void)envp;
}

void ft_init_term(t_ctx *ctx)
{
	const pid_t	mypid = ft_getpid();

	if (ft_setpgid (0, mypid) < 0)
		ft_dprintf(STDERR_FILENO,
				   "child setpgid (%ld to %ld)", (long)mypid, (long)0);
	ctx->parent_tpgrp = ft_tcgetpgrp(STDIN_FILENO);
	dup2(STDIN_FILENO, SHELL_TTY_FILENO);
	ft_give_terminal_to(mypid);
}

/**
 * https://stackoverflow.com/questions/61452938/
 * https://docs.rtems.org/
 * 	releases/4.5.1-pre3/toolsdoc/gdb-5.0-docs/readline/readline00030.html
 */
int	ft_sh_init_interactive(t_ctx **ctx, char **envp)
{
	t_ctx	*new;

	new = (t_ctx *) malloc(sizeof(t_ctx));
	if (!new)
	{
		errno = ENOMEM;
		return (-1);
	}
	new->ps0 = PS0;
	new->ps1 = PS1;
	new->ps2 = PS2;
	new->status_code = 0;
	ft_sh_init_builtin_ops(&new->ops);
	ft_sh_parse_env_map(&new->env_map, envp);
	*ctx = global_ctx(new, SET_VAL);
	ft_init_term(*ctx);
	ft_sh_set_signal(*ctx);
	return (0);
}
