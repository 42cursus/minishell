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

void	ft_sh_init_welcome(void);
int		ft_sh_execute(t_obj_arr *ops, t_ctx *ctx);

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
		line = ft_sh_read_line(ctx);
		if (line)
		{
			if (*line != 0)
			{
				add_history(line);
				if (ft_sh_split_line(line, ctx))
					break;
				status = ft_sh_execute(ops, ctx);
			}
			else
				free(line);
		}
		else
			printf("\n");
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
		global->envp = envp;
		exitcode = ft_sh_loop(global, ops);
	}
	else
		exitcode = EXIT_FAILURE;
	ft_sh_destroy_ctx(global);
	global = NULL;
	return (exitcode);
}
