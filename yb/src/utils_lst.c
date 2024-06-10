/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:32:51 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 17:45:58 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del(void *item)
{
	(void)item;
}

void	print_list(void *content)
{
	t_btree	*tmp;

	tmp = (t_btree *)content;
	print_node(tmp->item);
}
