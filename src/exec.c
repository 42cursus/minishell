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
		if (dup)
		{
			str = ft_strtok_r(dup, ":", &sptr);
			while ((not_found == -1) && str)
			{
				ft_snprintf(pathname, PATH_MAX, "%s/%s", str, ctx->argv[0]);
				//TODO: early break
				not_found = access(pathname, X_OK);
				str = ft_strtok_r(NULL, ":", &sptr);
			}
			free(dup);
		}
	}
	return (not_found);
}

static void shell_redirect_stdin(scmd_t *s)
{
	int fd;
	char path[PATH_MAX];

	ft_snprintf(path, sizeof(path), "%s", s->in->string);
	if (s->in->next_part)
		ft_strncat(path, get_word(s->in->next_part, s->ctx), 1024);
	fd = open(path, O_RDONLY, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void shell_redirect_stdout(scmd_t *s)
{
	char path[PATH_MAX];
	char *word;

	ft_snprintf(path, PATH_MAX, "%s", s->out->string);
	if (s->out->next_word)
	{
		if (s->out->next_part)
		{
			word = get_word(s->out->next_part, s->ctx);
			ft_strcat(path, word);
			free(word);
		}
	}

	int flags = O_WRONLY | O_CREAT;

	if (s->err || s->io_flags == IO_OUT_APPEND)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;

	ft_dprintf(STDERR_FILENO, "opening file : \"%s\" with permissions %d", path,(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	int fd = open(path, flags, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));

	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void shell_redirect_stderr(scmd_t *s)
{
	int fd;
	int flags;
	char path[1024];

	ft_snprintf(path, sizeof(path), "%s", s->err->string);

	if (s->err->next_part)
		strcat(path, get_word(s->err->next_part, s->ctx));

	flags = O_WRONLY | O_CREAT;
	if (s->out || s->io_flags == IO_REGULAR)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;

	fd = open(path, flags, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));

	dup2(fd, STDERR_FILENO);
	close(fd);
}

int ft_sh_launch(t_cmd_node *cmd, t_ctx *ctx)
{
	pid_t	pid;
	int		status = -1;

	if (!ft_sh_lookup_pathname(ctx) && cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			if (cmd->redirects_in)
				ft_shell_redirect_stdin(cmd);
			if (cmd->redirects_out)
				ft_shell_redirect_stdout(cmd);
			if (cmd->redirects_err_in)
				ft_shell_redirect_stderr_in(cmd);
			if (cmd->redirects_err)
				ft_shell_redirect_stderr(cmd);
			if (execve(ctx->pathname, ctx->argv,
					   ft_sh_render_envp(ctx)))
				return (perror("ft_sh: error in execve"), SHELL_EXIT);
		}
		else if (pid < 0)
			return (perror("ft_sh: error forking"), SHELL_EXIT);
		else
		{
			waitpid(pid, &status, WUNTRACED);        // Parent process
			while (!WIFEXITED(status) && !WIFSIGNALED(status))
				waitpid(pid, &status, WUNTRACED);
		}
	}
	else
		printf("%s: command not found\n", ctx->argv[0]);
	return (status);
}

int ft_sh_launch2(t_ctx *ctx, scmd_t *s)
{
	pid_t	pid;
	int		status = -1;

	if (!ft_sh_lookup_pathname(ctx) && s)
	{
		pid = fork();
		if (pid == 0)
		{
			if (s->in)
				shell_redirect_stdin(s);
			if (s->out)
				shell_redirect_stdout(s);
			if (s->err)
				shell_redirect_stderr(s);
			if (execve(ctx->pathname, ctx->argv,
					   ft_sh_render_envp(ctx)))    // Child process
				return (perror("ft_sh: error in execve"), SHELL_EXIT);
		}
		else if (pid < 0)
			return (perror("ft_sh: error forking"), SHELL_EXIT);
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
			ret_val = ft_sh_launch2(ctx, NULL);
	}
	return (ret_val);
}
