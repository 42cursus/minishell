/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellast.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <sbelcher@student.42london.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:20:53 by sbelcher          #+#    #+#             */
/*   Updated: 2024/12/05 14:17:57 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <readline.h>
#include "libft.h"

#define MAX_TOKENS 1024

typedef enum e_token_type {
	TOKEN_COMMAND,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_IN,
	TOKEN_HERE_DOC,
	TOKEN_APPEND,
	TOKEN_STRING,
	TOKEN_BLANK,
	TOKEN_END,
	TOKEN_VAR
} t_token_type;

typedef struct s_token {
	t_token_type type;
	char *value;
} t_token;

typedef enum e_state {
	INITIAL,
	READING_WORD,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE,
	CHECK_APPEND,
	CHECK_HERE_DOC,
	READING_WHITESPACE,
	END,
	VARIABLE
} t_state;

typedef struct s_lexer {
	char buffer[1024];
	int buf_index;
	char	curent_string;
	t_token *tokens[MAX_TOKENS];
	int token_count;
} t_lexer;

typedef enum e_node_type {
	NODE_COMMAND,
	NODE_ARGUMENT,
	NODE_PIPE,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_IN,
	NODE_HERE_DOC,
	NODE_FILENAME,
	NODE_APPEND
} t_node_type;

typedef struct s_ast_node {
	int type;
	char *value;
	struct s_ast_node *left;
	struct s_ast_node *right;
	struct s_ast_node *next;
} t_ast_node;

t_token		*create_token(t_token_type type, const char *value, t_lexer *lexer);
void		flush_buffer(t_lexer *lexer, t_token_type type);
void		free_tokens(t_lexer *lexer);
t_state		handle_initial(char c, t_lexer *lexer);
t_state		handle_reading_word(char c, t_lexer *lexer);
t_state		handle_in_single_quote(char c, t_lexer *lexer);
t_state		handle_variable(char c, t_lexer *lexer);
t_state		handle_in_double_quote(char c, t_lexer *lexer);
t_state		handle_check_append(char c, t_lexer *lexer);
t_state		handle_check_here_doc(char c, t_lexer *lexer);
t_state		handle_reading_whitespace(char c, t_lexer *lexer);
const char	*get_idstring(int token);
t_ast_node	*parse_redirection(t_token **tokens, int *token_pos);
t_ast_node	*parse_command_ast(t_token **tokens, int *token_pos);
t_ast_node	*parse_pipeline(t_token **tokens, int *token_pos);
t_ast_node	*create_node(t_node_type type, const char *value, t_ast_node *left, t_ast_node	*right, t_ast_node *next);
void		print_ast(t_ast_node *node, int depth);
void		free_ast(t_ast_node *node);
void		parse_command_details(t_token **tokens, int *token_pos, t_ast_node *command_node);

#endif
