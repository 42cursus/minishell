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
 * https://www.unix.com/programming/112624-readline-problems.html
 *
 * @return
 */
static int	ft_readline_event_hook(void)
{
	t_ctx *const	ctx = global_ctx(NULL, GET_VAL);

	if (g_received_signal_num == SIGINT)
	{
		g_received_signal_num = 0;
		if (ctx && ctx->prompt_type == PS_HERE)
		{
			ctx->received_signal_num = SIGINT;
			rl_replace_line("", 0);
			rl_done = 1;
		}
		else if (ctx && ctx->prompt_type == PS_REGULAR)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
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

__attribute__((unused))
static int	ft_sh_split_and_parse(const char *line,
									t_ast_node **node, t_ctx *ctx)
{
	int			errcode;
	char		*substr1;
	char		*substr2;
	char		*start;
	char		*end;
	t_ast_node	*root;
	t_ast_node	*root2;
	int			substr_len;

	line = "(echo 123) && ls";
	start = ft_strchr(line, '(') + 1;
	end = ft_strrchr(line, ')');
	substr1 = ft_strndup(start, end - start);
	substr_len = ft_snprintf(NULL, 0, "()%s", end);
	substr2 = (char *) malloc(sizeof(char) * (substr_len + 1));
	ft_snprintf(substr2, substr_len, "ls%s", end + 1);
	ft_do_parse(substr1, &root2, ctx);
	errcode = ft_do_parse(substr2, &root, ctx);
	free(root->left);
	root->left = root2;
	*node = root;
	return (errcode);
}

static int	ft_sh_loop(t_ctx *ctx)
{
	char			*line;
	t_ast_node		*ast;
	int				errcode;

	rl_event_hook = ft_readline_event_hook;
	ft_sh_init_welcome();
	ctx->argv = NULL;
	ctx->argc = 0;
	history_offset = history_length;
	while (ctx->last_status_code != SHELL_EXIT)
	{
		ast = NULL;
		line = ft_sh_read_line(ctx, PS_REGULAR);
		if (line && *line != 0)
		{
			add_history(line);
			ft_memset(&ctx->hd, 0, sizeof(t_here_arr));
			ctx->hd.size = HEREDOC_ARRAY_SIZE;
			errcode = ft_do_parse(line, &ast, ctx);
			if (!errcode && ft_sh_collect_heredocs(ctx))
			{
				{
					ft_printf("\n\nAbstract Syntax Tree:\n");
					print_ast(ast, 0);
					ast->ctx = ctx;
					//TODO: int attr on SHLVL
					ctx->last_status_code = ft_sh_execute_command(ast, 0);
				}
			}
			if (ast != NULL)
				free_ast(ast);
			unlink_herefiles(ctx);
			free(line);
		}
		else
			ctx->last_status_code = SHELL_EXIT;
	}
	rl_clear_history();
	return (ctx->last_status_code);
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
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
	{
		if (ft_sh_init_interactive(&global, envp) == -1)
			exit(EXIT_FAILURE);
		global->argv = argv;
		global->argc = argc;
		global->envp = envp;
		exitcode = ft_sh_loop(global);
		ft_sh_destroy_ctx(global);
	}
	else
		ft_sh_init_noninteractive(&global, envp);
	global = NULL;
	return (exitcode);
}
