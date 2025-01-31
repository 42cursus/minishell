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

int	herefile_varname(int i, char *var, char *line)
{
	int		bi;
	char	c;

	bi = 0;
	while (line[++i] != '\0' && bi < ARG_MAX)
	{
		c = line[i];
		if (bi == 0 && (c == '?' || c == '$'))
		{
			var[bi++] = c;
			break ;
		}
		else if ((bi == 0 && (ft_isalpha(c) || c == '_'))
			|| ((bi > 0) && (ft_isalnum(c) || c == '_')))
			var[bi++] = c;
		else if (bi == 0 && !ft_isalpha(c))
			return (1);
		else
			break ;
	}
	var[bi] = '\0';
	if (bi > 0)
		return (0);
	return (1);
}

int	unlink_herefiles(t_ctx *ctx)
{
	const char	*filename;
	int			i;
	int			err;

	if (ctx && !ctx->hd.already_unlinked)
	{
		i = -1;
		while (++i < ctx->hd.size)
		{
			filename = ctx->hd.entries[i].filename;
			err = unlink(filename);
			if (err != 0)
				return (1);
		}
		ctx->hd.already_unlinked = 1;
	}
	return (0);
}

void	herefile_expansion(int fd, const char *varname, t_ctx *ctx)
{
	const char	*value;

	if (ft_strcmp(varname, "$") == 0)
		ft_putnbr_fd(ft_getpid(), fd);
	else if (ft_strcmp(varname, "?") == 0)
		ft_putnbr_fd(ctx->status_code, fd);
	else
	{
		value = ft_sh_env_map_get_val(varname, ctx);
		if (value != NULL)
			ft_putstr_fd(value, fd);
	}
}

/**
 * https://www.unix.com/programming/112624-readline-problems.html
 *
 * @return
 */
static int	event(void)
{
	if (rl_pending_signal())
	{
		(void)NULL;
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
static int ft_sh_split_and_parse(const char *line, t_ast_node **node, t_ctx *ctx)
{
	int errcode;
	char *substr1;
	char *substr2;
	char *start;
	char *end;

	t_ast_node *root;
	t_ast_node *root2;

	line = "(echo 123) && ls";
//	line = "() && ls";

	start = ft_strchr(line, '(') + 1;
	end = ft_strrchr(line, ')');

	substr1 = ft_strndup(start, end - start);
	int substr_len = ft_snprintf(NULL, 0, "()%s", end);
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
	char		*line;
	t_ast_node	*ast;
	int			errcode;

	rl_event_hook = event;
	ft_sh_init_welcome();
	ctx->argv = NULL;
	ctx->argc = 0;
	history_offset = history_length;
	while (ctx->status_code != SHELL_EXIT)
	{
		ast = NULL;
		line = ft_sh_read_line(ctx, PS_REGULAR);
		if (line)
		{
			if (*line != 0)
			{
				add_history(line);
				ft_memset(&ctx->hd, 0, sizeof(t_here_arr));
				ctx->hd.size = HEREDOC_ARRAY_SIZE;

				errcode = ft_do_parse(line, &ast, ctx);

				if (errcode == 0)
				{
					if (ft_sh_collect_heredocs(ctx))
					{
						ft_printf("\n\nAbstract Syntax Tree:\n");
						print_ast(ast, 0);
						ast->ctx = ctx;
						ctx->status_code = ft_sh_execute_command(ast, 0, NULL);
					}
				}
				if (ast != NULL)
					free_ast(ast);
				unlink_herefiles(ctx);
			}
			free(line);
		}
		else
			ctx->status_code = SHELL_EXIT;
	}
	rl_clear_history();
	return (ctx->status_code);
}

/**
 * https://brennan.io/2015/01/16/write-a-shell-in-c/
 * https://www.geeksforgeeks.org/making-linux-shell-c/
 */
int	main(int argc, char **argv, char **envp)
{
	int			exitcode;
	t_ctx		*global;

	exitcode = 0;
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
