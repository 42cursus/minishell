/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 13:11:31 by abelov            #+#    #+#             */
/*   Updated: 2025/01/24 13:11:32 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_err(t_error err)
{
	if (err.assertion)
	{
		ft_perrorf( "minishell: %s", err.description);
		ft_dprintf(STDERR_FILENO,
			"on %s at %s:%d\n", err.func, err.file, err.line);
		return (-1);
	}
	return (0);
}
