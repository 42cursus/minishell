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
	(void)ctx;
	(void)info;
}

static t_obj_arr *ft_sh_init_builtin_ops(t_obj_arr **ops)
{
	static t_obj_arr	obj;
	static t_shell_op	builtins[] = {
	{.inst = "pwd", .fun = ft_pwd},
	{.inst = "env", .fun = ft_env},
	{.inst = "cd", .fun = ft_chdir},
	{.inst = "echo", .fun = ft_echo},
	{.inst = "export", .fun = ft_export},
	{.inst = "unset", .fun = ft_unset}
	};

	obj.base = builtins;
	obj.elem_size = sizeof(builtins[0]);
	obj.total_elems = sizeof(builtins) / sizeof(builtins[0]);
	obj.cmp_fun = ft_sh_op_cmp;
	ft_qsort_obj(&obj);
	*ops = &obj;
	return (0);
}

void ft_reset_sighandlers(t_ctx *ctx)
{
	int		i;
	char	(*const signals)[4] = &(char[4]) {
		SIGTERM,
		SIGINT,
		SIGQUIT,
		SIGWINCH
	};

	i = -1;
	while(++i < 4)
	{
		if (sigaction((int)(*signals)[i],
					  &ctx->old_act[(int)(*signals)[i]], NULL))
		{
			ft_sh_destroy_ctx(ctx);
			exit(EXIT_FAILURE);
		}
	}
}

void	ft_set_signal_handlers(t_ctx *const ctx)
{
	int		i;
	char	(*const signals)[4] = &(char[4]) {
		SIGTERM,
		SIGINT,
		SIGQUIT,
		SIGWINCH
	};

	i = -1;
	while(++i < 4)
	{
		if ((int)(*signals)[i] == SIGINT)
		{
			ctx->act[SIGINT].sa_flags = SA_SIGINFO | SA_RESTART;
			ctx->act[SIGINT].sa_sigaction = &ft_sh_sigint_handler;
		}
		else
			ctx->act[(int)(*signals)[i]].sa_handler = SIG_IGN;
		if (sigaction((int)(*signals)[i], &ctx->act[(int)(*signals)[i]],
					  &ctx->old_act[(int)(*signals)[i]]))
		{
			ft_sh_destroy_ctx(ctx);
			exit(EXIT_FAILURE);
		}
	}
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

	if (mypid != ft_getpgrp() && ft_setpgid (0, mypid) < 0)
		ft_dprintf(STDERR_FILENO, "ft_init_term setpgid (%d to %d) %d", mypid, 0, errno);
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

	new = (t_ctx *)ft_calloc(1, sizeof(t_ctx));
	if (!new)
	{
		errno = ENOMEM;
		return (-1);
	}
	new->ps0 = PS0;
	new->ps1 = PS1;
	new->ps2 = PS2;
	new->last_status_code = 0;
	ft_sh_init_builtin_ops(&new->ops);
	ft_sh_parse_env_map(&new->env_map, envp);
	*ctx = global_ctx(new, SET_VAL);
	ft_init_term(*ctx);
	ft_set_signal_handlers(*ctx);
	return (0);
}
