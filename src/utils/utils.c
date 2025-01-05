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

char *ft_get_word(t_wrd *wrd, t_ctx *ctx)
{
	t_sh_var *var = NULL;
	char *string = NULL;
	int string_length = 0;

	const char *substring = "";

	while (wrd != NULL)
	{
		if (wrd->expand == true)
		{
			var = ft_sh_env_map_get_entry(wrd->value, ctx);
			if (var && var->v)
				substring = var->v;
		}
		else
			substring = wrd->value;

		int substring_length = ft_strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		if (string == NULL)
		{
			fprintf(stderr, "(%s:%d, %s): ", __FILE__, __LINE__, __func__);
			perror("Error allocating word string.");
			exit(1);
		}

		string[string_length] = '\0';
		strcat(string, substring);

		string_length += substring_length;

		wrd = wrd->next_part;
	}

	return (string);
}

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

char **ft_get_argv(t_cmd_node *cmd, int *size, t_ctx *ctx)
{
	char **argv;
	int argc;

	t_wrd *param;
	argc = 1;

	param = cmd->args->next_word;
	while (param != NULL)
	{
		argc++;
		param = param->next_word;
	}

	argv = calloc(argc + 1, sizeof(char *));
	if (argv == NULL)
	{
		fprintf(stderr, "(%s:%d, %s): ", __FILE__, __LINE__, __func__);
		perror("Error allocating argv.");
		exit(1);
	}

	param = cmd->args;
	argc = 0;
	while (param != NULL)
	{
		char *word = ft_get_word(param, ctx);

		if (word == NULL)
		{
			fprintf(stderr, "(%s:%d, %s): ", __FILE__, __LINE__, __func__);
			perror("Error retrieving word.");
			exit(1);
		}
		argv[argc++] = word;
		param = param->next_word;
	}

	*size = argc;

	return (argv);
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
