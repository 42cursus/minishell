/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 00:49:25 by abelov            #+#    #+#             */
/*   Updated: 2024/07/03 00:49:26 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Return 1 if this token is a legal shell `identifier'; that is, it consists
 * solely of letters, digits, and underscores, and does not begin with a
 * digit.
 * @see 3.235 Name:
 * https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html
 */
int	ft_sh_is_legal_identifier(const char *name)
{
	const char		*s;
	unsigned char	c;

	if (!name)
		return (0);
	c = *name;
	if (!c || ((ft_isalpha(c) || (c == '_')) == 0))
		return (0);
	s = name + 1;
	while (*s != 0)
	{
		c = *s++;
		if ((ft_isalnum(c) || c == '_') == 0)
			return (0);
	}
	return (1);
}

int	ft_unset(t_ctx *ctx)
{
	int		i;
	int		retval;
	char	*name;

	i = 0;
	retval = 0;
	while (++i < ctx->argc)
	{
		name = ctx->argv[i];
		if (ft_sh_is_legal_identifier(name))
			ft_sh_env_map_unbind_var((t_sh_var){.k = name}, ctx);
		else
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: unset: `%s': not a valid identifier\n", name);
			retval = EXECUTION_FAILURE;
		}
	}
	return (retval);
}
