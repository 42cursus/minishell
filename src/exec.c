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
#include <fcntl.h>
#include "minishell.h"
#include "cmd.h"
#include "utils.h"

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
	if (*(ctx->argv[0]) == '/' || *(ctx->argv[0]) == '.')
		not_found = access(ft_strncpy(pathname, ctx->argv[0], PATH_MAX), X_OK);
	else
	{
		dup = ft_strdup(ft_sh_env_map_get_val("PATH", ctx));
		str = ft_strtok_r(dup, ":", &sptr);
		while ((not_found == -1) && str)
		{
			snprintf(pathname, PATH_MAX, "%s/%s", str, ctx->argv[0]);
			not_found = access(pathname, X_OK);
			str = ft_strtok_r(NULL, ":", &sptr);
		}
		free(dup);
//		if (dup)
//		{
//			str = ft_strtok_r(dup, ":", &sptr);
//			while ((not_found == -1) && str)
//			{
//				snprintf(pathname, PATH_MAX, "%s/%s", str, ctx->argv[0]);
//				not_found = access(pathname, X_OK);
//				str = ft_strtok_r(NULL, ":", &sptr);
//			}
//			free(dup);
//		}
	}
	return (not_found);
}

static void shell_redirect_stdin(simple_cmd_t *s)
{
	char path[1024];

	snprintf(path, sizeof(path), "%s", s->in->string);

	if (s->in->next_part)
		strcat(path, get_word(s->in->next_part, s->ctx));

	int fd = open(path, O_RDONLY, 0644);

	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void shell_redirect_stdout(simple_cmd_t *s)
{
	char path[PATH_MAX];

	snprintf(path, PATH_MAX, "%s", s->out->string);

	if (s->out->next_part)
		ft_strcat(path, get_word(s->out->next_part, s->ctx));

	int flags = O_WRONLY | O_CREAT;

	if (s->err || s->io_flags == IO_OUT_APPEND)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;

	int fd = open(path, flags, 0644);

	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void shell_redirect_stderr(simple_cmd_t *s)
{
	int fd;
	int flags;
	char path[1024];

	snprintf(path, sizeof(path), "%s", s->err->string);

	if (s->err->next_part)
		strcat(path, get_word(s->err->next_part, s->ctx));

	flags = O_WRONLY | O_CREAT;
	if (s->out || s->io_flags == IO_REGULAR)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;

	fd = open(path, flags, 0644);

	dup2(fd, STDERR_FILENO);
	close(fd);
}

int ft_sh_launch(t_ctx *ctx, simple_cmd_t *s)
{
	pid_t	pid;
	int		status = -1;

	if (!ft_sh_lookup_pathname(ctx) && s)
	{
		pid = fork();
		if (pid == 0)
		{
			if (execve(ctx->pathname, ctx->argv,
					   ft_sh_render_envp(ctx)))    // Child process
			{
				if (s->in)
					shell_redirect_stdin(s);

				if (s->out)
					shell_redirect_stdout(s);

				if (s->err)
					shell_redirect_stderr(s);

				perror("ft_sh: error in execve");
				return (SHELL_EXIT);
			}
		}
		else if (pid < 0)
		{
			perror("ft_sh: error forking");            // Error forking
			return (SHELL_EXIT);
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
	return (!status);
}

int	ft_sh_execute(t_obj_arr *ops, t_ctx *ctx)
{
	int 		ret_val;
	t_shell_op	*op;

	ret_val = 0;
	if (ctx->argc > 0)
	{
		op = ft_bsearch_obj(&(t_shell_op) {.instruction = ctx->argv[0]}, ops);
		if (op != NULL)
			ret_val = op->fun(ctx);
		else
			ret_val = ft_sh_launch(ctx, NULL);
	}
	return (ret_val);
}
