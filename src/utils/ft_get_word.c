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

char *ft_get_word(t_wrd *wrd, t_ctx *ctx)
{
	t_sh_var	*var = NULL;
	char		*string = NULL;
	int			string_length = 0;
	const char	*substring = "";
	int			substring_length;

	while (wrd != NULL)
	{
		substring = wrd->value;
		if (wrd->expand == true)
		{
			var = ft_sh_env_map_get_entry(wrd->value, ctx);
			if (var && var->v)
				substring = var->v;
		}
		substring_length = (int)ft_strlen(substring);
		string = ft_reallocarray(string, string_length,
								 string_length + substring_length + 1,
								 sizeof(char));
		if (string == NULL)
		{
			ft_dprintf(STDERR_FILENO, "on %s at %s:%d", __FILE__, __LINE__, __func__);
			errno = ENOMEM;
			perror("Error allocating word string.");
			exit(EXIT_FAILURE);
		}
		string[string_length] = '\0';
		ft_strcat(string, substring);
		string_length += substring_length;
		wrd = wrd->next_part;
	}
	return (string);
}
