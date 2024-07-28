/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 18:20:28 by abelov            #+#    #+#             */
/*   Updated: 2024/06/28 18:20:29 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigsegv(int signal)
{
	(void)signal;
	ft_putstr("> "FT_CYAN".SIGSEGV"FT_RESET"\n");
	exit(EXIT_SUCCESS);
}

void	check(bool succes)
{
	if (succes)
		ft_putstr("> "FT_GREEN".OK "FT_RESET"\n");
	else
		ft_putstr("> "FT_RED".KO "FT_RESET"\n");
}
