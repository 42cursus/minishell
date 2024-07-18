/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_expand_vars.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:36:23 by abelov            #+#    #+#             */
/*   Updated: 2024/07/18 23:21:07 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

enum  { VAR_MAX = 256 };

void	ft_sh_expand_vars(char **args)
{
	int		i;
	char	*arg;
	char	*expanded;
	char	*dst;
	char	*src;

	char	var_name[VAR_MAX];
	char	*var_value;
	char	*var_start;

	i = -1;
	while (args[++i] != NULL)
	{
		arg = args[i];
		expanded = malloc(ft_strlen(arg) + 1);
		dst = expanded;
		src = arg;
		while (*src)
		{
			if (*src == '$' && *(src + 1) && ft_isalnum(*(src + 1)))
			{
				src++; /* Skip the '$' */
				var_start = src;
				while (*src && (ft_isalnum(*src) || *src == '_'))
					src++;
				ft_strncpy(var_name, var_start, src - var_start);
				var_name[src - var_start] = '\0';
				var_value = getenv(var_name);
				if (var_value)
				{
					ft_strcpy(dst, var_value);
					dst += ft_strlen(var_value);
				}
			}
			else
				*dst++ = *src++;
		}
		*dst = '\0';
		free(args[i]);
		args[i] = expanded;
	}
}
