/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 00:39:50 by abelov            #+#    #+#             */
/*   Updated: 2025/01/22 00:39:51 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

__attribute__((always_inline))
static inline char	**ft_do_realloc(char **string, int slen, int sublen)
{
	const size_t	newlen = slen + sublen + 1;

	*string = ft_reallocarray(*string,
			slen, newlen, sizeof(char));
	return (string);
}

static const char	*ft_sh_do_expansion(const char *key, t_ctx *ctx)
{
	t_sh_var	*var;
	const char	*substring;

	substring = "";
	var = ft_sh_env_map_get_entry(key, ctx);
	if (var && var->v)
		substring = var->v;
	return (substring);
}

char	*ft_get_word(t_wrd *wrd, t_ctx *ctx)
{
	char		*string;
	int			slen;
	const char	*substring;
	int			sublen;

	slen = 0;
	string = NULL;
	while (wrd != NULL)
	{
		substring = wrd->value;
		if (wrd->to_expand)
			substring = ft_sh_do_expansion(wrd->value, ctx);
		sublen = (int)ft_strlen(substring);
		if (!ft_do_realloc(&string, slen, sublen))
			return (ft_perrorf(
					"minishell: %s", "Error allocating word string."), NULL);
		string[slen] = '\0';
		ft_strlcat(string, substring, slen + sublen + 1);
		slen += sublen;
		wrd = wrd->next_part;
	}
	return (string);
}
