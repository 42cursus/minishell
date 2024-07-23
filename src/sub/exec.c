/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 21:36:47 by abelov            #+#    #+#             */
/*   Updated: 2024/07/23 21:36:48 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "test.h"

int	ft_sh_lookup_pathname(t_ctx *ctx)
{
	char	*str;
	char	*dup;
	char	*sptr;
	int 	not_found;
	char	*pathname;

	pathname = ctx->pathname;
	not_found = -1;
	pathname[0] = '\0';
	if (ctx->argv[0][0] == '/' || ctx->argv[0][0] == '.')
		not_found = access(ft_strncpy(pathname, ctx->argv[0], PATH_MAX), X_OK);
	else
	{
		dup = ft_strdup(ft_sh_env_map_get_entry("PATH", ctx)->v);
		str = ft_strtok_r(dup, ":", &sptr);
		while ((not_found == -1) && str)
		{
			snprintf(pathname, PATH_MAX, "%s/%s", str, ctx->argv[0]);
			not_found = access(pathname, X_OK);
			str = ft_strtok_r(NULL, ":", &sptr);
		}
		free(dup);
	}
	return (not_found);
}

int ft_sh_launch(t_ctx *ctx)
{
	pid_t	pid;
	int		status;

	if (!ft_sh_lookup_pathname(ctx))
	{
		pid = fork();
		if (pid == 0)
		{
			if (execve(ctx->pathname,
					   ctx->argv, ctx->envp))    // Child process
			{
				perror("ft_sh: error in execve");
				ft_sh_destroy_ctx(ctx);
				exit(EXIT_FAILURE);
			}
		}
		else if (pid < 0)
		{
			perror("ft_sh: error forking");            // Error forking
			ft_sh_destroy_ctx(ctx);
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &status, WUNTRACED);        // Parent process
			while (!WIFEXITED(status) && !WIFSIGNALED(status))
				waitpid(pid, &status, WUNTRACED);
		}
	}
	else
		printf("%s: command not found\n", ctx->argv[0]);
	return (0);
}

int	ft_sh_execute(t_obj_arr *ops, t_ctx *ctx)
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
