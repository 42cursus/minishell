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

int	ft_shell_handle_redirect(t_wrd *wrd, int fd_redir, t_ctx *ctx, t_dir d)
{
	const char	*word = NULL;
	int			flags;
	int			fd;
	t_error		err;

	if (!wrd)
		return (EXECUTION_FAILURE);
	while (wrd)
	{
		flags = O_RDONLY;
		if (d == OUT)
			flags = O_WRONLY | O_CREAT | wrd->redir_flag;
		word = ft_get_word(wrd, ctx);
		fd = open(word, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		err = (t_error){
			.assertion = (fd < 0 && errno == ENOENT), .description = word,
			.func = __func__, .line = __LINE__, .file = __FILE__};
		if (ft_handle_err(err))
			return (free((void *) word), EXECUTION_FAILURE);
		free((void *) word);
		if (wrd->next_word)
			close(fd);
		wrd = wrd->next_word;
	}
	return ((void)dup2(fd, fd_redir), (void)close(fd), EX_OK);
}
