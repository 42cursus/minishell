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

void sigsegv(int signal)
{
	(void) signal;
	ft_putstr("> "FT_CYAN".SIGSEGV"FT_RESET"\n");
	exit(EXIT_SUCCESS);
}

void check(bool succes)
{
	if (succes)
		ft_putstr("> "FT_GREEN".OK "FT_RESET"\n");
	else
		ft_putstr("> "FT_RED".KO "FT_RESET"\n");
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

char *get_word(word_t *s, t_ctx *ctx)
{
	t_sh_var *var = NULL;
	char *string = NULL;
	int string_length = 0;

	const char *substring = "";
	int substring_length = 0;

	while (s != NULL)
	{
		if (s->expand == true)
		{
			var = ft_sh_env_map_get_entry(s->string, ctx);
			if (var && var->v)
				substring = var->v;
		}
		else
			substring = s->string;

		substring_length = ft_strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		DIE(string == NULL, "Error allocating word string.");

		string[string_length] = '\0';
		strcat(string, substring);

		string_length += substring_length;

		s = s->next_part;
	}

	return (string);
}

char **get_argv(simple_cmd_t *command, int *size, t_ctx *ctx)
{
	char **argv;
	int argc;

	word_t *param;

	argc = 1;

	param = command->params;
	while (param != NULL)
	{
		param = param->next_word;
		argc++;
	}

	argv = calloc(argc + 1, sizeof(char *));
	DIE(argv == NULL, "Error allocating argv.");

	argv[0] = get_word(command->verb, ctx);
	DIE(argv[0] == NULL, "Error retrieving word.");

	param = command->params;
	argc = 1;
	while (param != NULL)
	{
		argv[argc] = get_word(param, ctx);
		DIE(argv[argc] == NULL, "Error retrieving word.");

		param = param->next_word;
		argc++;
	}

	*size = argc;

	return argv;
}
