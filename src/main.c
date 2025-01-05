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

#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <errno.h>
#include "minishell.h"

void	ft_sh_init_welcome(void);
int		ft_sh_execute(t_obj_arr *ops, t_ctx *ctx);
int		ft_sh_tokenize(t_obj_arr *ops, t_ctx *ctx);

void	collect_heredocs(t_ctx *ctx)
{
	int				fd;
	int				i;
	char			*line;
	const int		flags = O_WRONLY | O_CREAT;
	const mode_t	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	HeredocEntry	*en;

	if (ctx == NULL)
		return;
	i = -1;
	while(++i < ctx->hd.size)
	{
		en = &ctx->hd.entries[i];
		fd = open(en->filename, flags, mode);
		if (fd < 0) break;
		line = ft_sh_read_line(ctx, "> ");

		while(ft_strcmp(line, en->delimiter))
		{
			
			dprintf(fd, "%s\n", line);
			line = ft_sh_read_line(ctx, "> ");
		}
		close(fd);
	}
}

int	ft_sh_loop2(t_ctx *ctx)
{
	const char	*line;
	int			status;
	t_ast_node	*ast = NULL;

	ft_sh_init_welcome();
	status = 0;
	ctx->argv = NULL;
	ctx->argc = 0;

	line = "<<HERE cat | grep $?hdbf";

	ctx->hd.ss = 0;
	ctx->hd.size = 1024;
	ft_memset(ctx->hd.entries, 0, sizeof(HeredocEntry) * HEREDOC_ARRAY_SIZE);
	int errcode = parse_pipeline(line, &ast, ctx);
	if (!errcode)
	{
		if (!ast)
			ft_printf("Error: Failed to parse the command.\n");
		else
		{
			collect_heredocs(ctx);
			// ft_pritintf("\n\nAbstract Syntax Tree:\n");
			// print_ast(ast, 0);
			ast->ctx = ctx;
			status = traverse_and_exec_the_ast(ast, 0, NULL);
			free_ast(ast);
		}
	}
	else
		status = SHELL_EXIT;

	return (status);
}

int	ft_sh_loop(t_ctx *ctx)
{
	char	*line;
	int		status;
	cmd_t *root = NULL;
	t_ast_node *ast = NULL;

	ft_sh_init_welcome();
	status = 0;
	ctx->argv = NULL;
	ctx->argc = 0;
	using_history();
	while (status != SHELL_EXIT)
	{
		line = ft_sh_read_line(ctx, NULL);
		if (line)
		{
			if (*line != 0)
			{
				add_history(line);
				ctx->hd.ss = 0;
				ctx->hd.size = 1024;
				ft_memset(ctx->hd.entries, 0, sizeof(HeredocEntry) * HEREDOC_ARRAY_SIZE);
				int errcode = parse_pipeline(line, &ast, ctx);
				if (errcode)
				{
					// clean and break
					status = SHELL_EXIT;
					break;
				}
				if (!ast)
					ft_printf("Error: Failed to parse the command.\n");
				else
				{
					collect_heredocs(ctx);
					ft_printf("\n\nAbstract Syntax Tree:\n");
					print_ast(ast, 0);
					ast->ctx = ctx;
					status = traverse_and_exec_the_ast(ast, 0, NULL);
				}

// 				root = NULL;
// 				/* We might have not read the entire line... */
// 				if (!parse_line(line, &root))
// 				{
// 					/* There was an error parsing the command. */
// 					fprintf(stderr, "Error while parsing command!\n");
// 					status = -1;
// 				}
// 				else
// 				{
// 					root->ctx = ctx;
// 					root->ast = ast;
// 					status = traverse_and_exec_the_ast2(root, 0, NULL);
// //					status = exec_ast(ast, 0, NULL);
// 					free_parse_memory();
// 				}
				free_ast(ast);
			}
			free(line);
		}
		else
			status = SHELL_EXIT;
	}
	return (status);
}

/**
 * https://brennan.io/2015/01/16/write-a-shell-in-c/
 * https://www.geeksforgeeks.org/making-linux-shell-c/
 */
int main(int argc, char **argv, char **envp)
{
	int 		exitcode;
	t_ctx		*global;
	t_obj_arr	*ops;

	if (ft_sh_do_init(&global, envp, &ops) != -1)
	{
		global->argv = argv;
		global->argc = argc;
		global->ops = ops;
		exitcode = ft_sh_loop(global);
	}
	else
		exitcode = EXIT_FAILURE;
	ft_sh_destroy_ctx(global);
	global = NULL;
	return (exitcode);
}
