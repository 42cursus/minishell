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
# define MINISHELL_H

# include <stdbool.h>
# include <error.h>
# include <errno.h>
# include <stdio.h>
# include <signal.h>
# include <termios.h>
# include <sysexits.h>
# include <asm-generic/errno.h>
# include <linux/limits.h>
# include "libft.h"
# include "parser.h"

# include <sys/stat.h>
# include <fcntl.h>

# include <signal.h>
# include <readline/readline.h>

# include <stdlib.h>
# include <string.h>
# include <ctype.h>

# define MAX_TOKENS 1024
# define MAX_PATH_LEN 1024


typedef enum e_token_type
{
	TOKEN_COMMAND,
	T_WORD,
	TOKEN_PIPE,
	TOKEN_STRING,
	TOKEN_BLANK,
	TOKEN_END,
	T_VAR,
	T_REDIRECT_STDOUT,
	TOKEN_REDIRECT_STDIN,
	TOKEN_HERE_DOC,
	TOKEN_APPEND,
	TOKEN_REDIRECT_STDERR,
	TOKEN_REDIRECT_IN_2,
	TOKEN_HERE_DOC_2,
	TOKEN_APPEND_2,
	TOKEN_MAX
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	bool			hereexpand;
	char			*value;
}	t_token;

typedef enum e_state
{
	INITIAL,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE,
	CHECK_APPEND,
	CHECK_HERE_DOC,
	END
}	t_state;

typedef struct s_lexer
{
	char	buffer[1024];
	int		buf_index;
	char	curent_string;
	char	*line;
	int		line_iter;
	bool	first_blank;
	bool	here_eof;
	int		err;
	int		heredoc_index;
	t_token	*t[MAX_TOKENS];
	int		tokens_size;
	int		token_iter;
}	t_lexer;

int			exec_ast(t_ast_node *node, int level, t_ast_node *parent);

int			unlink_herefiles(t_ctx *ctx);
int			handle_parser_err(int errcode, t_lexer *lexer);
int			scan_the_line(const char *line, t_lexer *lexer);
t_token		*create_token(t_token_type type, const char *value, t_lexer *lexer);
void		flush_buffer(t_lexer *lexer, t_token_type type);
t_state		handle_initial(t_lexer *lexer);
t_state		handle_symbol(t_lexer *lexer, t_state state);
t_state		handle_in_single_quote(t_lexer *lexer);
t_state		exit_variable(t_lexer *l);
t_state		handle_variable(t_lexer *lexer);
t_state		handle_in_double_quote(t_lexer *lexer);
t_state		handle_check_append(t_lexer *lexer, int i);
t_state		handle_check_here_doc(t_lexer *lexer, int i);
t_state		handle_reading_whitespace(t_lexer *lexer);
const char	*get_idstring(int token);
void		free_tokens(t_lexer *lexer);
void		parse_redirection(int *tp, t_ast_node *p, t_ctx *ctx, t_lexer *l);
t_ast_node	*parse_command(t_token **t, int *tp, t_ctx *ctx, t_lexer *l);
int			parse_pipeline(const char *line, t_ast_node **root, t_ctx *ctx);
t_ast_node	*create_node(t_node_type type, t_token *t, t_ast_node *parent);
void		find_redir_list(t_wrd *redir, t_token_type type, t_cmd_node *cmd);
void		print_ast(t_ast_node *node, int depth);
void		free_ast(t_ast_node *node);
void		free_wrd(t_wrd *word);
void		print_arguments(t_wrd *arguments, int depth);
void		print_redirections(t_wrd *redir, int depth, t_token_type rt);
void		skip_blanks(t_token **ts, int *tp, t_wrd *last, t_lexer *l);
void		create_wrd(t_wrd *word, t_token *token, t_token_type rt);
t_ast_node	*pipeline_loop(t_lexer *lexer, t_ctx *ctx);
void		parse_command_loop(int *tp, t_ctx *ctx, t_ast_node *cn, t_lexer *l);
void		print_tokens(t_lexer *lexer);
void		end_of_heredoc_check(t_lexer *lexer);
void		herefile_lexing(int fd, char *line, bool quotes);
int			herefile_varname(int i, char *var, char *line);
void		herefile_expansion(int fd, char *var);
t_state		create_pid_token(t_lexer *lexer);
int			ft_getpid(void);
int			here_doc_cat(t_wrd *here, t_lexer *l);
char		*hd_cat_loop(t_wrd *here, size_t len, t_lexer *l);
void		disable_ctrl_c_printing(void);
void		restore_ctrl_c_printing(void);
void		handle_sigint(int sig);
void		collect_heredocs(t_ctx *ctx);

int			traverse_and_exec_the_ast2(cmd_t *c, int level, cmd_t *father);
int			traverse_and_exec_the_ast(t_ast_node *cmd, int level, t_ast_node *father);
void		ft_shell_redirect_stdin(t_cmd_node *cmd);
void		ft_shell_redirect_stdout(t_cmd_node *cmd);
void		ft_shell_redirect_stderr(t_cmd_node *cmd);
void		ft_shell_redirect_stderr_in(t_cmd_node *cmd);

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
char		*ft_sh_read_line(t_ctx *ctx, const char *fmt);
int			ft_sh_do_init(t_ctx	**ctx, char **envp, t_obj_arr **ops);
int			ft_sh_op_cmp(const void *a, const void *b);
int			ft_sh_var_cmp(const void *a, const void *b);
int			ft_sh_parse_env_map(t_obj_arr *map, char **env_tab);
char		**ft_sh_render_envp(t_ctx *ctx);
int			ft_sh_launch(t_cmd_node *cmd, t_ctx *ctx);
int			ft_sh_launch2(t_ctx *ctx, simple_cmd_t *s);
int			ft_sh_env_map_del_entry(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_add_entry(t_sh_var var, t_ctx *ctx);
t_sh_var	*ft_sh_env_map_get_entry(const char *key, t_ctx *ctx);
char		*ft_sh_env_map_get_val(const char *key, t_ctx *ctx);
int			ft_sh_env_map_get_index(const char *key, t_ctx *ctx);
int			ft_sh_env_map_bind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_unbind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_destroy_ctx(t_ctx *ctx);

#endif //MINISHELL_H
