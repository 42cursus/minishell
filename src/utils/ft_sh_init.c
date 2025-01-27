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

static void	ft_sh_sig_handler(int sig, siginfo_t *info, void *ctx)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	return ;
	(void)ctx;
	(void)info;
}

/**
 * Readline C: force return of certain text in readline()
 *
 * https://stackoverflow.com/questions/53165704/
 */
static void	ft_hd_sig_handler(int sig, siginfo_t *info, void *ctx)
{
	int	sipid;

	if (sig == SIGINT)
	{
		g_received_signal_num = SIGINT;
		rl_replace_line("", 0);
		rl_done = 1;
	}
	return ;
	sipid = info->si_pid;
	(void)ctx;
	(void)sipid;
}

static int	do_init_ops(t_obj_arr **ops)
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
	ctx->hd_act.sa_flags = SA_SIGINFO | SA_RESTART;
	ctx->hd_act.sa_sigaction = &ft_hd_sig_handler;
	sigemptyset(&ctx->hd_act.sa_mask);
	sigaddset(&ctx->hd_act.sa_mask, SIGINT);
	ctx->act.sa_flags = SA_SIGINFO | SA_RESTART;
	ctx->act.sa_sigaction = &ft_sh_sig_handler;
	sigemptyset(&ctx->act.sa_mask);
	sigaddset(&ctx->act.sa_mask, SIGINT);
	if (sigaction(SIGINT, &ctx->act, NULL))
	{
		ft_sh_destroy_ctx(ctx);
		exit(EXIT_FAILURE);
	}
	signal(SIGQUIT, SIG_ERR);
}

/**
 * https://stackoverflow.com/questions/61452938/
 * https://docs.rtems.org/
 * 	releases/4.5.1-pre3/toolsdoc/gdb-5.0-docs/readline/readline00030.html
 */
int	ft_sh_do_init(t_ctx **ctx, char **envp, t_obj_arr **ops)
{
	t_ctx	*new;

	do_init_ops(ops);
	new = (t_ctx *) malloc(sizeof(t_ctx));
	if (!new)
		return (-1);
	ft_sh_parse_env_map(&new->env_map, envp);
	new->env_map.cmp_fun = ft_sh_var_cmp;
	ft_qsort_obj(&new->env_map);
	*ctx = new;
	ft_sh_set_signal(*ctx);
	return (0);
}
