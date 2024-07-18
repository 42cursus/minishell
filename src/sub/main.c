/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 14:04:07 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 14:04:08 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <history.h>
#include <readline.h>
#include "test.h"

int		ft_sh_split_line(char *input, t_ctx *ctx);
void	ft_sh_init_welcome(void);

char *ft_sh_lookup_pathname(t_ctx *ctx)
{
	char	*str;
	char	*dup;
	char	*sptr;
	char	*join;
	int 	not_found;
	char 	*pathname;

	not_found = 1;
	pathname = ctx->argv[0];
	dup = ft_strdup(ft_shell_env_map_get_entry("PATH", ctx)->v);
	str = ft_strtok_r(dup, ":", &sptr);
	while (not_found && str)
	{
		pathname = ft_strjoin2((const char *[]){str, ctx->argv[0]}, 2, "/");
		not_found = access(pathname, X_OK);
		str = ft_strtok_r(NULL, ":", &sptr);
	}
	free(dup);
	return (pathname);
}

int ft_sh_launch(t_ctx *ctx)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(ft_sh_lookup_pathname(ctx),
				   ctx->argv, ctx->envp) == -1)	// Child process
			perror("ft_sh: error in execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("ft_sh: Error forking");			// Error forking
	else
	{
		waitpid(pid, &status, WUNTRACED);		// Parent process
		while (!WIFEXITED(status) && !WIFSIGNALED(status))
			waitpid(pid, &status, WUNTRACED);
	}
	return (0);
}

int ft_sh_execute(t_obj_arr *ops, t_ctx *ctx)
{
	int 		ret_val;
	t_shell_op	*op;

	if (ctx->argv == NULL || ctx->argv[0] == NULL)
		return (0);
	op = ft_bsearch_obj(&(t_shell_op) {.instruction = ctx->argv[0]}, ops);
	if (op != NULL)
		ret_val = op->fun(ctx);
	else
		ret_val = ft_sh_launch(ctx);
	return (ret_val);
}

enum { MAXC = 128 };

char *ft_sh_read_line(void)
{
	char 	ps[MAXC] = "";
	char	*pwd;
	char	*p;
	char	*line;
	char	*fmt;
	static int		count = 1;

	line = NULL;
	p = getenv("USER");
	pwd = getenv("PWD");

	fmt = "[%d] "FT_GREEN"%s"FT_RESET"@"FT_BLUE"%s"FT_RESET"> ";
	sprintf(ps, fmt, count, p, pwd);
	line = readline(ps);
	count++;
	return (line);
}

int ft_sh_loop(t_ctx *ctx, t_obj_arr *ops)
{
	char	*line;
	int		status;

	ft_sh_init_welcome();
	status = 0;
//	if (ctx->argc > 1)
//		ctx->argv = ft_split(ctx->argv[1], ' ');
//	status = ft_sh_execute(ops, ctx);
	ctx->argv = NULL;
	using_history();    /* initialize history */
	while (!status)
	{
		line = ft_sh_read_line();
		if (line && *line)
		{
			add_history(line);
			if(ft_sh_split_line(line, ctx))
				break ;
			status = ft_sh_execute(ops, ctx);
		}
		else
			printf("\n");
		free(line);
	}
	return (status);
}

/**
 * https://brennan.io/2015/01/16/write-a-shell-in-c/
 * https://www.geeksforgeeks.org/making-linux-shell-c/
 * https://stackoverflow.com/questions/38792542/readline-h-history-usage-in-c
 */

int main(int argc, char **argv, char **envp)
{
	t_ctx *global;
	t_obj_arr *ops;


	if (do_init(&global, envp, &ops) == -1)
		exit(-1);
	global->argv = argv;
	global->argc = argc;
	global->envp = envp;
	ft_sh_loop(global, ops);
	ft_sh_destroy_ctx(&global);
	free(ops);
	return (EX_OK);
}
