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

#include "minishell.h"

char	*ft_sh_do_expand(t_ctx *ctx, char *dst, const char *var_name)
{
	char	*var_value;

	var_value = ft_sh_env_map_get_val(var_name, ctx);
	if (var_value)
	{
		ft_strcpy(dst, var_value);
		dst += ft_strlen(var_value);
	}
	return (dst);
}

char	*ft_sh_expand_single_var(t_ctx *ctx, char *src)
{
	char	*new;
	char	*dst;
	char	var_name[VAR_MAX];
	char	*var_start;

	new = ft_strnew(ft_strlen(src) + 1);
	if (!new)
		return (NULL);
	dst = new;
	while (*src)
	{
		if (*src == '$' && *(src + 1) && (ft_isalnum(*(src + 1)) || *(src + 1) == '_'))
		{
			var_start = ++src; /* Skip the '$' */
			while (*src && (ft_isalnum(*src) || *src == '_'))
				src++;
			ft_strncpy(var_name, var_start, src - var_start);
			var_name[src - var_start] = '\0';
			dst = ft_sh_do_expand(ctx, dst, var_name);
		}
		else
			*dst++ = *src++;
	}
	*dst = '\0';
	return (new);
}

void	ft_sh_expand_vars(t_ctx *ctx)
{
	int		i;
	char	*arg;
	char	*expanded;

	i = -1;
	while (ctx->argv[++i] != NULL)
	{
		arg = ctx->argv[i];
		expanded = ft_sh_expand_single_var(ctx, arg);
		free(ctx->argv[i]);
		ctx->argv[i] = expanded;
	}
}
