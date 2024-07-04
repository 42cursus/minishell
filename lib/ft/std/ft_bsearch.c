/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bsearch.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 20:41:26 by abelov            #+#    #+#             */
/*   Updated: 2024/06/23 20:41:26 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_bsearch_obj(const void *key, t_obj_arr *obj)
{
	size_t		lower;
	size_t		upper;
	size_t		idx;
	const void	*ptr;
	int			comparison;

	lower = 0;
	upper = obj->total_elems;
	while (lower < upper)
	{
		idx = (lower + upper) / 2;
		ptr = (void *)(((const char *) obj->base)
				+ (idx * obj->elem_size));
		comparison = (*obj->cmp_fun)(key, ptr);
		if (comparison < 0)
			upper = idx;
		else if (comparison > 0)
			lower = idx + 1;
		else
			return ((void *) ptr);
	}
	return (NULL);
}
