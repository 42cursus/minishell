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

void	disable_ctrl_c_printing(void)
{
    struct termios	term;

	tcgetattr(0, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(0, TCSANOW, &term);
}

void	restore_ctrl_c_printing(void)
{
    struct termios	term;

    tcgetattr(0, &term);
	term.c_lflag |= ECHOCTL;
    tcsetattr(0, TCSANOW, &term);
}

int	herefile_varname(int i, char *var, char *line)
{
	int		bi;
	char	c;

	bi = 0;
	while (line[++i] != '\0')
	{
		c = line[i];
		if (bi == 0 && (ft_isalpha(c) || c == '_'))
			var[bi++] = c;
		else if (bi == 0 && !ft_isalpha(c) && c != '_')
			return (1);
		else if ((bi > 0) && (ft_isalnum(c) || c == '_'))
			var[bi++] = c;
		else
			break ;
	}
	var[bi] = '\0';
	if (bi > 0)
		return (0);
	return (1);
}

int		unlink_herefiles(t_ctx *ctx)
{
	const char	*filename;
	int			i;
	int			err;

	i = -1;
	err = 0;
	while(++i < ctx->hd.size)
	{
		filename = ctx->hd.entries[i].filename;
		err = unlink(filename);
		if (err != 0)
			return (1);
	}
	return (0);
}

void	herefile_expansion(int fd, char *var)
{
	const char	*value;

	value = getenv(var);
	if (value != NULL)
		ft_dprintf(fd, "%s", value);
	else
	{
		ft_dprintf(fd, "$");
		ft_dprintf(fd, "%s", var);
	} 
}

void	herefile_lexing(int fd, char *line, bool quotes)
{
	int		i;
	int		e;
	char	var[1000];

	i = -1;
	while (line[++i] != '\0' && quotes == false)
	{
		if (line[i] == '$')
		{
			e = herefile_varname(i, var, line);
			i += (ft_strlen(var));
			if (e == 1)
				write(fd, "$", 1);
			else if (e == 0)
				herefile_expansion(fd, var); 
		}
		else
			write(fd, &line[i], 1);
	}
	if (quotes == true)
		ft_dprintf(fd, "%s", line);
	write(fd, "\n", 1);
}

void handle_sigint(int sig)
{

    printf("\nCaught SIGINT (Ctrl+C)!\n");
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

	line = "cat '' '' '' ''$HOME/../$USER/.emacs'' '' ''$PWD | "
		   "sleep > $HOME/\"tmp\"/stdout.txt 2>> $HOME/tmp/stderr.txt";

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
			 ft_printf("\n\nAbstract Syntax Tree:\n");
			 print_ast(ast, 0);
			ast->ctx = ctx;
			status = traverse_and_exec_the_ast(ast, 0, NULL);
			free_ast(ast);
		}
	}
	else
		status = SHELL_EXIT;

	return (status);
}


int event(void) { return(0); }

int	ft_sh_loop(t_ctx *ctx)
{
	char	*line;
	int		status;
	cmd_t *root = NULL;
	t_ast_node *ast = NULL;


	// rl_getc_function = getc;
	rl_event_hook=event;

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
				unlink_herefiles(ctx);
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
