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

void ft_shell_handle_redirect(t_wrd *wrd, int fd_redir, t_ctx *ctx, t_dir d)
{
	char		path[MAX_PATH_LEN];
	const char	*word = NULL;
	int			flags;
	int			fd;

	if (!wrd)
		return ;
	while (wrd)
	{
		flags = O_RDONLY;
		if (d == OUT)
			flags = O_WRONLY | O_CREAT | wrd->redir_flag;
		word = ft_get_word(wrd, ctx);
		ft_strncpy(path, word, MAX_PATH_LEN);
		fd = open(word, flags, DEFAULT_MODE);
		if (ft_handle_err((t_error) {
			.assertion = (fd < 0 && errno == ENOENT), .description = word,
			.func = __func__, .line = __LINE__, .file = __FILE__}))
			return (free((void *) word));
		free((void *) word);
		if (wrd->next_word)
			close(fd);
		wrd = wrd->next_word;
	}
	dup2(fd, fd_redir);
	close(fd);
}
