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
	char path[1024];

	snprintf(path, sizeof(path), "%s", s->out->string);

	if (s->out->next_part)
		strcat(path, get_word(s->out->next_part, s->ctx));

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
	char path[1024];

	snprintf(path, sizeof(path), "%s", s->err->string);

	if (s->err->next_part)
		strcat(path, get_word(s->err->next_part, s->ctx));

	int flags = O_WRONLY | O_CREAT;

	if (s->out || s->io_flags == IO_REGULAR)
		flags = flags | O_TRUNC;
	else
		flags = flags | O_APPEND;

	int fd = open(path, flags, 0644);

	dup2(fd, STDERR_FILENO);
	close(fd);
}

#ifdef USE_SHELL_REDIRECT_FUN
static void shell_redirect(simple_cmd_t *s)
{
	int stdin_fd = dup(STDIN_FILENO);
	int stdout_fd = dup(STDOUT_FILENO);
	int stderr_fd = dup(STDERR_FILENO);

	s->in ? shell_redirect_stdin(s) : NULL;
	s->out ? shell_redirect_stdout(s) : NULL;
	s->err ? shell_redirect_stderr(s) : NULL;

	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	dup2(stderr_fd, STDERR_FILENO);
}
#endif

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
				s->in ? shell_redirect_stdin(s) : NULL;
				s->out ? shell_redirect_stdout(s) : NULL;
				s->err ? shell_redirect_stderr(s) : NULL;

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

	if (ctx->argv == NULL || ctx->argv[0] == NULL)
		return (0);
	op = ft_bsearch_obj(&(t_shell_op) {.instruction = ctx->argv[0]}, ops);
	if (op != NULL)
		ret_val = op->fun(ctx);
	else
		ret_val = ft_sh_launch(ctx, NULL);
	return (ret_val);
}
