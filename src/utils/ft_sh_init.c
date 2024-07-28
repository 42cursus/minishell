/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 23:04:03 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 14:18:21 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_init_ops(t_obj_arr **ops)
{
	static t_obj_arr	obj;
	static t_shell_op	builtins[] = {
	{.instruction = "pwd", .fun = ft_pwd},
	{.instruction = "env", .fun = ft_env},
	{.instruction = "cd", .fun = ft_chdir},
	{.instruction = "echo", .fun = ft_echo},
	{.instruction = "export", .fun = ft_export},
	{.instruction = "unset", .fun = ft_unset},
	{.instruction = "exit", .fun = ft_exit}
	};

	obj.base = builtins;
	obj.elem_size = sizeof(builtins[0]);
	obj.total_elems = sizeof(builtins) / sizeof(builtins[0]);
	obj.cmp_fun = ft_sh_op_cmp;
	ft_qsort_obj(&obj);
	*ops = &obj;
	return (0);
}

void	sig_handler(int sig, siginfo_t *info, void *ctx)
{
	int					sipid;

	sipid = info->si_pid;
	(void)ctx;
	(void)sipid;
	if (sig == SIGINT)
	{
		printf("SIGINT\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		printf("SIGQUIT\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_sh_set_signal(t_ctx *const *ctx)
{
	t_sigaction	act;

	act.sa_flags = SA_SIGINFO | SA_RESTART;
	act.sa_sigaction = &sig_handler;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigaddset(&act.sa_mask, SIGQUIT);
	if (sigaction(SIGINT, &act, NULL) || sigaction(SIGQUIT, &act, NULL))
	{
		ft_sh_destroy_ctx(*ctx);
		exit(EXIT_FAILURE);
	}
	signal(SIGSEGV, sigsegv);
}

/**
 * https://stackoverflow.com/questions/61452938/
 * https://docs.rtems.org/
 * 	releases/4.5.1-pre3/toolsdoc/gdb-5.0-docs/readline/readline00030.html
 */
int	ft_sh_do_init(t_ctx **ctx, char **envp, t_obj_arr **ops)
{
	t_ctx	*new;

	ft_sh_set_signal(ctx);
	do_init_ops(ops);
	new = (t_ctx *) malloc(sizeof(t_ctx));
	if (!new)
		return (-1);
	ft_sh_parse_env_map(&new->env_map, envp);
	new->env_map.cmp_fun = ft_sh_var_cmp;
	ft_qsort_obj(&new->env_map);
	new->fdio.in = 0;
	new->fdio.out = 1;
	new->fdio.err = 2;
	*ctx = new;
	return (0);
}

/**
 * Greeting shell during startup
 */
void	ft_sh_init_welcome(void)
{
	char	*username;

//	printf("\033[H\033[J");
	printf("\n******************************************");
	printf("\n\n\t***** 42 MINISHELL *****");
	printf("\n\t* USE AT YOUR OWN RISK *");
	printf("\n\n******************************************");
	username = getenv("USER");
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
}
