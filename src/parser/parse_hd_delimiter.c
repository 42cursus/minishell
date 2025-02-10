/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hd_delimiter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:30:49 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/10 17:30:51 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*hd_cat_loop(t_wrd *here, size_t len, t_lexer *l)
{
	char	*str;

	str = ft_calloc(sizeof(char), len + 1);
	if (str == NULL)
		l->err = ALLOC_FAILURE;
	while (here)
	{
		ft_strcat(str, here->value);
		here = here->next_part;
	}
	return (str);
}

int	here_doc_cat(t_wrd *here, t_lexer *l)
{
	t_wrd		*origin;
	char		*str;
	size_t		len;

	origin = here;
	len = 0;
	if (here->next_part)
	{
		while (here)
		{
			len += ft_strlen(here->value);
			here = here->next_part;
		}
		here = origin;
		str = hd_cat_loop(here, len, l);
		if (!str)
			return (HD_CAT_FAILURE);
		free((void *)origin->value);
		free_wrd(origin->next_part);
		origin->value = ft_strdup(str);
		free(str);
		origin->next_part = NULL;
	}
	return (0);
}

void	remove_non_compliant_chars(char *buf, int buf_size)
{
	char	*dst;

	dst = buf;
	while (buf_size--)
	{
		if (ft_isalnum((unsigned char) *buf))
			*dst++ = *buf;
		buf++;
	}
	*dst = '\0';
	return ;
}

int	add_random_numbers_to_str(char *str_buf, int rand_count)
{
	char	buf[OPTIMISTIC + 1];
	int		fd;
	ssize_t	bytes_read;
	int		len;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		errno = COULDNT_OPEN_URANDOM;
		return (-1);
	}
	len = 0;
	bytes_read = 1;
	while (rand_count > len && bytes_read > 0)
	{
		bytes_read = read(fd, buf + len, OPTIMISTIC - len);
		remove_non_compliant_chars(buf + len, OPTIMISTIC - len);
		len = (int)ft_strnlen(buf, OPTIMISTIC);
	}
	close(fd);
	ft_strncat(str_buf, buf, rand_count);
	return (EX_OK);
}

int	create_here_file(t_wrd *here, t_hd_entry *entry, bool expand)
{
	int		olderrno;
	ssize_t	error_code;

	olderrno = errno;
	error_code = 0;
	if (here)
	{
		entry->quotes = expand;
		ft_snprintf(entry->filename, FILENAME_BUF_SIZE,
			"/tmp/heredoc_%d_", ft_getpid_c());
		add_random_numbers_to_str(entry->filename, 20);
		error_code = errno;
		ft_strcpy(entry->delimiter, here->value);
		free((void *)here->value);
		here->value = NULL;
		here->value = ft_strdup(entry->filename);
	}
	errno = olderrno;
	return ((int)error_code);
}
