/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:26:43 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/12 14:26:45 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_idstring(int token)
{
	static const char	*lt[TOKEN_MAX] = {
		"TOKEN_DUMMY",
		"TOKEN_OR",
		"TOKEN_AND",
		"TOKEN_PIPE",
		"TOKEN_COMMAND",
		"TOKEN_WORD",
		"TOKEN_STRING",
		"TOKEN_BLANK",
		"TOKEN_END",
		"TOKEN_VAR",
		"TOKEN_REDIRECT_OUT",
		"TOKEN_REDIRECT_IN",
		"TOKEN_HERE_DOC",
		"TOKEN_APPEND",
		"TOKEN_REDIRECT_OUT_2",
		"TOKEN_REDIRECT_IN_2",
		"TOKEN_HERE_DOC_2",
		"TOKEN_APPEND_2"
	};

	return (lt[token]);
}

static void	ft_print_depth(int depth)
{
	int	i;

	i = -1;
	while (++i < depth + 1)
		ft_printf("  ");
}

void	print_tokens(t_lexer *lexer)
{
	int				i;
	t_token			*tok;
	const char		*type;
	t_token	*const	(*toks)[MAX_TOKENS] = &(lexer->tok.t);

	ft_printf("Tokens:\n");
	i = -1;
	while (++i < lexer->tok.tokens_size)
	{
		tok = (*toks)[i];
		type = get_idstring(tok->type);
		if (tok->value && ft_strlen(tok->value) == 0)
			ft_printf("  Type: \"%s\", Value: %s\n", type, "(empty)");
		else
			ft_printf("  Type: \"%s\", Value: %s\n", type, tok->value);
	}
}

void	ft_print_arg_node(t_wrd *arguments)
{
	t_wrd	*node;
	char	buf[PRINT_ARG_BUFF_SIZE];

	node = arguments;
	while (node)
	{
		*buf = '\0';
		if (node->value && node->to_expand)
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "expand(%s)", node->value);
		else if (node->value && ft_strlen(node->value) == 0)
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "(empty string)");
		else
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "%s", node->value);
		ft_printf("%s", buf);
		if (node->next_part)
			ft_printf("; ");
		node = node->next_part;
	}
	ft_printf("\n");
}

void	print_arguments(t_wrd *arguments, int depth)
{
	while (arguments)
	{
		ft_print_depth(depth);
		ft_printf("ARGUMENT: ");
		ft_print_arg_node(arguments);
		arguments = arguments->next_word;
	}
}
