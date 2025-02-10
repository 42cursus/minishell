#include "minishell.h"

typedef struct
{
	int token_id;
	const char *str;
} lt_entry;

lt_entry lt[] = {
	{ .token_id = TOKEN_DUMMY,		.str = "TOKEN_DUMMY" },
	{ .token_id = TOKEN_AND,		.str = "TOKEN_AND" },
	{ .token_id = TOKEN_OR,		.str = "TOKEN_OR" },
	{ .token_id = TOKEN_COMMAND,		.str = "TOKEN_COMMAND" },
	{ .token_id = T_WORD,				.str = "TOKEN_WORD" },
	{ .token_id = TOKEN_PIPE,			.str = "TOKEN_PIPE" },
	{ .token_id = T_REDIRECT_STDOUT,	.str = "TOKEN_REDIRECT_OUT" },
	{ .token_id = TOKEN_REDIRECT_STDIN,	.str = "TOKEN_REDIRECT_IN" },
	{ .token_id = TOKEN_HERE_DOC,		.str = "TOKEN_HERE_DOC" },
	{ .token_id = TOKEN_APPEND,			.str = "TOKEN_APPEND" },
	{ .token_id = TOKEN_STRING,			.str = "TOKEN_STRING" },
	{ .token_id = TOKEN_BLANK,			.str = "TOKEN_BLANK" },
	{ .token_id = T_VAR,				.str = "TOKEN_VAR" },
	{ .token_id = TOKEN_END,			.str = "TOKEN_END" },
	{ .token_id = TOKEN_REDIRECT_STDERR,		.str = "TOKEN_REDIRECT_OUT_2" },
	{ .token_id = TOKEN_REDIRECT_IN_2,			.str = "TOKEN_REDIRECT_IN_2" },
	{ .token_id = TOKEN_HERE_DOC_2,				.str = "TOKEN_HERE_DOC_2" },
	{ .token_id = TOKEN_APPEND_2,				.str = "TOKEN_APPEND_2" }
};

const char *get_idstring(int token)
{
	int i;
	const char *str = NULL;

	i = -1;
	while (++i < TOKEN_MAX)
	{
		if (lt[i].token_id == token)
		{
			str = lt[i].str;
			break ;
		}
	}
	return str;
}

void	print_tokens(t_lexer *lexer)
{
	printf("\n\nTokens:\n");
	lexer->line_iter = -1;
	while (lexer->tok.t[++(lexer->line_iter)] != NULL)
	{
		if (!lexer->tok.t[lexer->line_iter]->value)
		{
			printf("Token Type: \"%s\", Value: %s\n",
			   get_idstring(lexer->tok.t[lexer->line_iter]->type), "NULL");
		}
		else if (lexer->tok.t[lexer->line_iter]->value && ft_strlen(lexer->tok.t[lexer->line_iter]->value) == 0)
		{
			printf("Token Type: \"%s\", Value: %s\n",
			   get_idstring(lexer->tok.t[lexer->line_iter]->type), "(empty string)");
		}
		else
			printf("Token Type: \"%s\", Value: %s\n",
			   get_idstring(lexer->tok.t[lexer->line_iter]->type), lexer->tok.t[lexer->line_iter]->value);
	}
}

static void	ft_print_arg_node(t_wrd *arguments, char *buf)
{
	t_wrd	*cont_node;
	t_wrd	*node;

	cont_node = arguments;
	while (cont_node->next_part)
	{
		*buf = '\0';
		node = cont_node->next_part;
		if (node->value && node->expand)
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "expand(%s)", node->value);
		else if (node->value && ft_strlen(node->value) == 0)
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "(empty string)");
		else
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "%s", node->value);
		ft_printf("; %s", buf);
		cont_node = cont_node->next_part;
	}
	printf("\n");
}

void	print_arguments(t_wrd *arguments, int depth)
{
	char	buf[PRINT_ARG_BUFF_SIZE];
	int		i;

	while (arguments)
	{
		*buf = '\0';
		i = -1;
		while (++i < depth)
			ft_printf("  ");
		if (arguments->value && ft_strlen(arguments->value) == 0)
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "(empty string)");
		else if (arguments->value && arguments->expand)
			ft_snprintf(buf,
				PRINT_ARG_BUFF_SIZE, "expand(%s)", arguments->value);
		else
			ft_snprintf(buf, PRINT_ARG_BUFF_SIZE, "%s", arguments->value);
		ft_printf("ARGUMENT: %s", buf);
		ft_print_arg_node(arguments, buf);
		arguments = arguments->next_word;
	}
}

void	print_redirections(t_wrd *redir, int depth, t_token_type rt)
{
	const char	*type;
	int			i;

	if (rt == T_REDIRECT_STDOUT)
		type = ">";
	else if (rt == TOKEN_REDIRECT_STDERR)
		type = "2>";
	else if (rt == TOKEN_REDIRECT_STDIN)
		type = "<";
	else if (rt == TOKEN_REDIRECT_IN_2)
		type = "2<";
	while (redir)
	{
		i = -1;
		while (++i < depth)
			ft_printf("  ");
		printf("Redirection Type: %s", type);
		if (redir->redir_flag == O_APPEND)
			printf(">");
		printf(", Target: %s", redir->value);
		if (redir->expand == true)
			printf(" - TO EXPAND");
		while (redir->next_part)
		{
			if (redir->next_part->value)
				printf("; %s", redir->next_part->value);
			else
				printf("; %s", "NULL");
			if (redir->next_part->expand == true)
				printf(" - TO EXPAND");
			redir = redir->next_part;
		}
		printf("\n");
		redir = redir->next_word;
	}
}
