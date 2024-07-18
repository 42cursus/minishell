/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tab_get_size.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 22:16:45 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 22:42:54 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_tab_get_size(void const **tab)
{
	size_t	size;

	size = 0;
	while (*tab && ++size)
		tab++;
	return (size);
}
