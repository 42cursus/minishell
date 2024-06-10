/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:59:19 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 17:45:48 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_strtrim_and_free(t_btree *root)
{
	char	*result;

	result = ft_strtrim(root->item, " ");
	free(root->item);
	root->item = result;
}
