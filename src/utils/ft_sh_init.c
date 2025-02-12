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
void	ft_sh_sigint_handler(int sig, siginfo_t *info, void *ctx)
{
	if (sig == SIGINT)
		g_received_signal_num = SIGINT;
	return ;
	(void)ctx;
	(void)info;
}

t_obj_arr	*ft_sh_init_builtin_ops(t_obj_arr **ops)
{
	static t_obj_arr	obj;
	static t_shell_op	builtins[] = {
	{.inst = "pwd", .fun = ft_pwd},
	{.inst = "env", .fun = ft_env},
	{.inst = "cd", .fun = ft_chdir},
	{.inst = "echo", .fun = ft_echo},
	{.inst = "export", .fun = ft_export},
	{.inst = "exit", .fun = ft_exit_with_exitcode},
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

void	ft_reset_sighandlers(t_ctx *ctx)
{
	int				i;
	const	char	(*signals)[5] = &(char [5]){
		SIGTERM,
		SIGINT,
		SIGQUIT,
		SIGWINCH,
		SIGTSTP
	};

	i = -1;
	while (++i < (int)(sizeof((*signals)) / sizeof((*signals)[0])))
	{
		if (sigaction((int)(*signals)[i],
			&ctx->old_act[(int)(*signals)[i]], NULL))
		{
			ctx->last_status_code = EXIT_FAILURE;
			exit(ft_sh_destroy_ctx(ctx));
		}
	}
}

void	ft_set_signal_handlers(t_ctx *const ctx)
{
	int				i;
	const	char	(*signals)[5] = &(char [5]){
		SIGTERM,
		SIGINT,
		SIGQUIT,
		SIGWINCH,
		SIGTSTP
	};

	i = -1;
	while (++i < (int)(sizeof((*signals)) / sizeof((*signals)[0])))
	{
		ctx->act[(int)(*signals)[i]].sa_handler = SIG_IGN;
		if ((int)(*signals)[i] == SIGINT)
		{
			ctx->act[SIGINT].sa_flags = SA_SIGINFO | SA_RESTART;
			ctx->act[SIGINT].sa_sigaction = &ft_sh_sigint_handler;
		}
		if (sigaction((int)(*signals)[i], &ctx->act[(int)(*signals)[i]],
			&ctx->old_act[(int)(*signals)[i]]))
		{
			ctx->last_status_code = EXIT_FAILURE;
			exit(ft_sh_destroy_ctx(ctx));
		}
	}
}

int	ft_sh_init_noninteractive(t_ctx **ctx, char **envp)
{
	return (0);
	(void)ctx;
	(void)envp;
}
