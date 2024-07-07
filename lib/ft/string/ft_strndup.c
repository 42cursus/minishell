/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:42:23 by abelov            #+#    #+#             */
/*   Updated: 2024/07/02 17:42:24 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * The ft_strndup() function is similar to ft_strdup, but copies at most nb
 * bytes. If s is longer than nb, only nb bytes are copied,
 * and a terminating null byte ('\0') is added.
 */
char	*ft_strndup(const char *s, unsigned int nb)
{
	char			*dest;
	const size_t	srclen = ft_strlen(s);

	dest = (char *)malloc(srclen + 1);
	if (dest == NULL)
		return (NULL);
	*dest = '\0';
	ft_strncpy(dest, s, nb);
	return (dest);
}
