/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:17:44 by abelov            #+#    #+#             */
/*   Updated: 2024/12/18 18:17:45 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * S_IRUSR: Read permission for the owner (0400 in octal).
 * S_IWUSR: Write permission for the owner (0200 in octal).
 * S_IRGRP: Read permission for the group (0040 in octal).
 * S_IROTH: Read permission for others (0004 in octal).
 */
void ft_shell_redirect_stdin(t_cmd_node *cmd)
{
	char			path[MAX_PATH_LEN];
	t_wrd			*wrd;
	t_wrd			*next;
	char 			*word;
	int				fd;
	const mode_t	mode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	char 			buf[PATH_MAX];

	wrd = cmd->redirects_in;
	while (wrd && wrd->next_word)
	{
		word = ft_get_word(wrd, cmd->ctx);
		fd = open(word, O_RDONLY, mode);
		if (fd < 0 && errno == ENOENT)
		{
			ft_snprintf(buf, PATH_MAX, "minishell: %s ", word);
			perror(buf);
			ft_dprintf(STDERR_FILENO, "on %s at %s:%d", __FILE__, __LINE__, __func__);
		}
	}

	ft_snprintf(path, sizeof(path), "%s", cmd->redirects_in->value);

	if (cmd->redirects_in->next_part)
		ft_strcat(path, ft_get_word(cmd->redirects_in->next_part, cmd->ctx));
	fd = open(path, O_RDONLY, mode);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void ft_shell_redirect_stdout(t_cmd_node *cmd)
{
	char path[PATH_MAX];

	ft_snprintf(path, PATH_MAX, "%s", cmd->redirects_out->value);

	if (cmd->redirects_out->next_word)
		if (cmd->redirects_out->next_part)
			ft_strcat(path, ft_get_word(cmd->redirects_out->next_part, cmd->ctx));

	int flags = O_WRONLY | O_CREAT;

	if (cmd->redirects_out->append)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;


	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	ft_dprintf(STDERR_FILENO, "opening file : \"%s\" with permissions %d", path, mode);
	int fd = open(path, flags, mode);

	dup2(fd, STDOUT_FILENO);
	close(fd);
}


void ft_shell_redirect_stderr_in(t_cmd_node *cmd)
{

}

void ft_shell_redirect_stderr(t_cmd_node *cmd)
{
	int fd;
	int flags;
	char path[1024];

	ft_snprintf(path, sizeof(path), "%s", cmd->redirects_err->value);

	if (cmd->redirects_err->next_part)
		ft_strcat(path, ft_get_word(cmd->redirects_err->next_part, cmd->ctx));

	flags = O_WRONLY | O_CREAT;
	if (cmd->redirects_err->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;

	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	fd = open(path, flags, mode);

	dup2(fd, STDERR_FILENO);
	close(fd);
}
