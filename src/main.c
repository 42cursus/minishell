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

#include <history.h>
#include <readline.h>
#include "minishell.h"
#include "cmd.h"

void	ft_sh_init_welcome(void);
int		ft_sh_execute(t_obj_arr *ops, t_ctx *ctx);
int		ft_sh_tokenize(t_obj_arr *ops, t_ctx *ctx);


//int main(void)
//{
//
//
//	char ps[1024];
//
//	char *line = NULL;
//
//	sprintf(ps, "> ");
//	line = readline(ps);
//	while (line)
//	{
//		t_lexer lexer = scan_the_Line(line);
//
//		int	token_pos = 0;
//		t_ast_node *ast = parse_pipeline(lexer.tokens, &token_pos);
//
//		if (!ast)
//			ft_printf("Error: Failed to parse the command.\n");
//		else
//		{
//			ft_printf("\n\nAbstract Syntax Tree:\n");
//			print_ast(ast, 0);
//		}
//		free_ast(ast);
//		free_tokens(&lexer);
//		free(line);
//		line = readline(ps);
//	}
//	free(line);
//	return (0);
//}

int	ft_sh_loop(t_ctx *ctx)
{
	char	*line;
	int		status;
	cmd_t *root = NULL;

	ft_sh_init_welcome();
	status = 0;
	ctx->argv = NULL;
	ctx->argc = 0;
	using_history();
	while (status != SHELL_EXIT)
	{
		line = ft_sh_read_line(ctx);
		if (line)
		{
			if (*line != 0)
			{
				add_history(line);
				root = NULL;
				/* We might have not read the entire line... */
				if (!parse_line(line, &root))
				{
					/* There was an error parsing the command. */
					fprintf(stderr, "Error while parsing command!\n");
					status = -1;
				}
				else
				{
					root->ctx = ctx;
					status = parse_command(root, 0, NULL);
					free_parse_memory();
				}
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
