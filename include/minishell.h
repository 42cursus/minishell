/* ************************************************************************** */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:14:26 by abelov            #+#    #+#             */
/*   Updated: 2024/11/15 15:20:53 by sbelcher          #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdbool.h>
# include <error.h>
# include <stdio.h>
# include <signal.h>
# include <sysexits.h>
# include <asm-generic/errno.h>
# include <linux/limits.h>
# include "libft.h"
# include "parser.h"

# include <signal.h>
# include <readline.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 1024

typedef enum e_token_type {
	TOKEN_COMMAND,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_ERR,
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
	NODE_CONTINUE,
	NODE_APPEND
} t_node_type;

typedef struct s_ast_node {
	t_node_type			type;
	char				*value;
	struct s_ast_node	*parent;
	struct s_ast_node	*arguments;
	struct s_ast_node	*redirects_in;
	struct s_ast_node	*redirects_out;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*word_continue;
	bool				expand;
} t_ast_node;

t_lexer		scan_the_Line(const char *line);
t_token     *create_token(t_token_type type, const char *value, t_lexer *lexer);
void        flush_buffer(t_lexer *lexer, t_token_type type);
void        free_tokens(t_lexer *lexer);
t_state     handle_initial(char c, t_lexer *lexer);
t_state     handle_reading_word(char c, t_lexer *lexer);
t_state     handle_in_single_quote(char c, t_lexer *lexer);
t_state     handle_variable(char c, t_lexer *lexer);
t_state     handle_in_double_quote(char c, t_lexer *lexer);
t_state     handle_check_append(char c, t_lexer *lexer);
t_state     handle_check_here_doc(char c, t_lexer *lexer);
t_state     handle_reading_whitespace(char c, t_lexer *lexer);
const char  *get_idstring(int token);
t_ast_node  *parse_redirection(t_token **tokens, int *token_pos, t_ast_node *parent);
t_ast_node  *parse_command(t_token **tokens, int *token_pos);
t_ast_node  *parse_pipeline(t_token **tokens, int *token_pos);
t_ast_node	*create_node(t_node_type type, char *value, t_ast_node *parent, t_token_type token);
t_ast_node	*create_redirection_node(t_token_type type, char *value, t_ast_node *parent, t_token_type token);
void        print_ast(t_ast_node *node, int depth);
void        free_ast(t_ast_node *node);
void        parse_command_details(t_token **tokens, int *token_pos, t_ast_node *command_node);
void        print_arguments(t_ast_node *arg_node, int depth);
void        print_redirections(t_ast_node *redir_node, int depth);
void		skip_blanks(t_token **tokens, int *token_pos, t_ast_node *last);

enum	{ VAR_MAX = 256 };
enum	{ MAXC = 128 };

typedef struct sigaction	t_sigaction;

/**
 * k => key
 * v => value
 * attrs => attributes
 */
typedef struct s_shell_var
{
	const char	*k;
	const char	*v;
	int 		attrs;
}	t_sh_var;



# define ATT_EXPORTED	0x0000001	/* export to environment */
# define ATT_READONLY	0x0000002	/* cannot change */
# define ATT_INTEGER	0x0000010	/* internal representation is int */
# define ATT_NOUNSET 	0x0002000	/* cannot unset */
# define ATT_NOASSIGN	0x0004000	/* assignment not allowed */
# define ATT_IMPORTED	0x0008000	/* came from environment */

/** Basic text colors
 * character 27 = 033 = 0x1b = ^[ = \e
 * CSI (Control Sequence Introducer) or CSI, the ESC [
 * 	(written as \e[ or \033[) is followed by any number (including none)
 * 	of "parameter bytes" in the range 0x30–0x3F (ASCII 0–9:;<=>?),
 * 	then by any number of "intermediate bytes" in the range 0x20–0x2F
 * 	(ASCII space and !"#$%&'()*+,-./),
 * 	then finally by a single "final byte" in the range 0x40–0x7E
 * 	(ASCII @A–Z[\]^_`a–z{|}~).
 */
#define FT_BLACK "\033[0;30m"
#define FT_RED "\e[0;31m"
#define FT_GREEN "\e[0;32m"
#define FT_YELLOW "\e[0;33m"
#define FT_BLUE "\e[:;34m"
#define FT_PURPLE "\e[0;35m"
#define FT_CYAN "\e[0;36m"
#define FT_WHITE "\e[0;37m"
#define FT_RESET "\e[0;m"

/* ---------- TESTS -------------------- */

void		check(bool succes);
void		ft_print_title(char *title);

typedef int	(*t_shell_fun)(t_ctx *);
typedef struct s_shell_op
{
	const char	*instruction;
	t_shell_fun	fun;
}	t_shell_op;

/* ---------- FUNCTIONS -------------------- */
int			ft_chdir(t_ctx *ctx);
int			ft_env(t_ctx *ctx);
int			ft_pwd(t_ctx *ctx);
int			ft_echo(t_ctx *ctx);
int			ft_export(t_ctx *ctx);
int			ft_unset(t_ctx *ctx);

/* ---------- UTILS -------------------- */
void		sigsegv(int signal);
int			ft_sh_split_line(char *input, t_ctx *ctx);
char		*ft_sh_read_line(t_ctx *ctx);
int			ft_sh_do_init(t_ctx	**ctx, char **envp, t_obj_arr **ops);
int			ft_sh_op_cmp(const void *a, const void *b);
int			ft_sh_var_cmp(const void *a, const void *b);
int			ft_sh_parse_env_map(t_obj_arr *map, char **env_tab);
char		**ft_sh_render_envp(t_ctx *ctx);
int ft_sh_launch(t_ctx *ctx, simple_cmd_t *s);
int			ft_sh_env_map_del_entry(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_add_entry(t_sh_var var, t_ctx *ctx);
t_sh_var	*ft_sh_env_map_get_entry(const char *key, t_ctx *ctx);
char		*ft_sh_env_map_get_val(const char *key, t_ctx *ctx);
int			ft_sh_env_map_get_index(const char *key, t_ctx *ctx);
int			ft_sh_env_map_bind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_unbind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_destroy_ctx(t_ctx *ctx);

#endif //MINISHELL_H
