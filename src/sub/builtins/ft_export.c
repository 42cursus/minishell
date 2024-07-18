/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 00:48:48 by abelov            #+#    #+#             */
/*   Updated: 2024/07/03 00:48:48 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "test.h"

int	ft_export(t_ctx *ctx)
{
	char 		*str;
	char		*tmp;
	char		*sptr;
	t_sh_var 	var;
	int			ret_val;

	ret_val = 0;
	var = (t_sh_var){.k = NULL, .v = NULL};
	str = ft_strdup(*(ctx->argv));
	while (str && !ft_strchr(str, '='))
	{
		var.k = ft_strtok_r(str, "=", &sptr);
		tmp = ft_strtok_r(NULL, "=", &sptr);
		if (tmp)
			var.v = ft_strdup(tmp);
		var.attrs = ATT_EXPORTED;
		str = *(ctx->argv);
	}
	if (str)
		return (FT_EXPORT_ERR_NOT_A_VAR);
	return (0);
}
