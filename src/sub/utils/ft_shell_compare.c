/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_compare.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:49:38 by abelov            #+#    #+#             */
/*   Updated: 2024/07/05 12:49:39 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	ft_shell_op_cmp(const void *a, const void *b)
{
	const t_shell_op	*op1 = a;
	const t_shell_op	*op2 = b;

	return (ft_strcmp(op1->instruction,  op2->instruction));
}

int	ft_shell_var_cmp(const void *a, const void *b)
{
	const t_sh_var	*var1 = a;
	const t_sh_var	*var2 = b;

	return (ft_strcmp(var1->k,  var2->k));
}

