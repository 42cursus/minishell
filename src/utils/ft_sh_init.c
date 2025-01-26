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

volatile sig_atomic_t	g_received_signal_num = 0;

static void	sig_handler(int sig, siginfo_t *info, void *ctx)
{
	int					sipid;

	sipid = info->si_pid;
	(void)ctx;
	(void)sipid;
	if (g_received_signal_num == 0)
	{
		if (sig == SIGINT)
		{
			g_global_hd = 0;
			rl_replace_line("", 0);
			rl_done = 1;
		}
		return ;
	}
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/**
 * Readline C: force return of certain text in readline()
 *
 * https://stackoverflow.com/questions/53165704/
 */
static void	hd_sig_handler(int sig, siginfo_t *info, void *ctx)
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

int	collect_heredocs_loop(t_ctx *ctx)
{
	const mode_t	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int				fd;
	char			*line;
	t_hd_entry		*en;
	int	i;
	int retcode;

	retcode = 1;
	i = -1;
	while (++i < ctx->hd.size && g_received_signal_num != SIGINT)
	{
		en = &ctx->hd.entries[i];
		fd = open(en->filename, O_WRONLY | O_CREAT, mode);
		if (fd < 0)
			break ;
		t_sigaction	act;
		t_sigaction	oldact;

		act.sa_flags = SA_SIGINFO | SA_RESTART;
		act.sa_sigaction = &hd_sig_handler;
		sigemptyset(&act.sa_mask);
		sigaddset(&act.sa_mask, SIGINT);

		g_received_signal_num = 0;

		if (sigaction(SIGINT, &act, &oldact))
		{
			ft_sh_destroy_ctx(ctx);
			exit(EXIT_FAILURE);
		}

		line = ft_sh_read_line(ctx, "> ");
		while (line && ft_strcmp(line, en->delimiter))
		{
			add_history(line);
			herefile_lexing(fd, line, en->quotes, ctx);
			line = (free(line), ft_sh_read_line(ctx, "> "));
			if (g_received_signal_num == SIGINT)
			{
				retcode = 0;
				break;
			}
		}
		if (sigaction(SIGINT, &oldact, &act))
		{
			ft_sh_destroy_ctx(ctx);
			exit(EXIT_FAILURE);
		}
		free(line);
		close(fd);
	}

	return (retcode);
}

int	collect_heredocs(t_ctx *ctx)
{
	if (!collect_heredocs_loop(ctx))
	{
		ctx->status_code = (-1);
		unlink_herefiles(ctx);
		g_received_signal_num = 0;
		return (0);
	}
	return (1);
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

static void	ft_sh_set_signal(t_ctx *const *ctx)
{
	t_sigaction	act;

	act.sa_flags = SA_SIGINFO | SA_RESTART;
	act.sa_sigaction = &sig_handler;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);


	if (sigaction(SIGINT, &act, NULL))
	{
		ft_sh_destroy_ctx(*ctx);
		exit(EXIT_FAILURE);
	}
	signal(SIGQUIT, SIG_ERR);
	return ;
	(void )ctx;
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
	*ctx = new;
	return (0);
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
