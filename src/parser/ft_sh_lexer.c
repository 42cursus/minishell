/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_lexer->c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:52:28 by abelov            #+#    #+#             */
/*   Updated: 2024/07/28 22:52:29 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_sh_tokenize(t_obj_arr *ops, t_ctx *ctx)
{
	(void)ops;
	int i;

	i = -1;
	while (++i < ctx->argc)
		ft_putstr_eol(ctx->argv[i], "\n");

	return (0);
}
