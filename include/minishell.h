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

# define _GNU_SOURCE

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

# include <sys/random.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

extern volatile sig_atomic_t	g_received_signal_num;

# define HEREFILE_BUF_LEN 80

# define BANNER "\
\n****************************************** \
\n\n\t***** 42 MINISHELL ***** \
\n\t* USE AT YOUR OWN RISK * \
\n\n******************************************"

# define MAX_TOKENS 1024
# define MAX_PATH_LEN 1024

# define ATT_EXPORTED	0x0000001	/* export to environment */
# define ATT_READONLY	0x0000002	/* cannot change */
# define ATT_INTEGER	0x0000010	/* internal representation is int */
# define ATT_NOUNSET 	0x0002000	/* cannot unset */
# define ATT_NOASSIGN	0x0004000	/* assignment not allowed */
# define ATT_IMPORTED	0x0008000	/* came from environment */

/* Values that can be returned by execute_command (). */
# define EXECUTION_FAILURE 1
# define EXECUTION_SUCCESS 0

/* Usage messages by builtins result in a return status of 2. */
# define EX_BADUSAGE	2
# define EX_MISCERROR	2
# define EX_SHELL_EXIT	-100

/* Special exit statuses used by the shell, internally and externally. */
# define EX_BINARY_FILE	126
# define EX_NOEXEC		126
# define EX_NOTFOUND	127

# define EX_BADSYNTAX	257	/* shell syntax error */
# define FT_USAGE		258	/* syntax error in usage like cd with many params */
# define EX_REDIRFAIL	259	/* redirection failed */
# define EX_BADASSIGN	260	/* variable assignment error */
# define EX_EXPFAIL		261	/* word expansion failed */

typedef enum e_token_type
{
	TOKEN_DUMMY,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_PIPE,
	TOKEN_COMMAND,
	T_WORD,
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

typedef struct s_toks
{
	t_token	*t[MAX_TOKENS];
	int		tokens_size;
	int		token_iter;
}	t_toks;

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
	t_toks	tok;
}	t_lexer;

typedef struct s_error
{
	int			assertion;
	int			errnum;
	int			fd;
	const char	*description;
	int			line;
	const char	*func;
	const char	*file;
}	t_error;
/*
 * S_IRUSR: Read permission for the owner (0400 in octal).
 * S_IWUSR: Write permission for the owner (0200 in octal).
 * S_IRGRP: Read permission for the group (0040 in octal).
 * S_IROTH: Read permission for others (0004 in octal).
 */
# define DEFAULT_MODE S_IRUSR
# define SHELL_TTY_FILENO 255

typedef enum e_direction
{
	IN = 0,
	OUT
}	t_dir;

typedef enum e_lombok
{
	GET_VAL = 0,
	SET_VAL
}	t_get_or_set;

void		ft_sh_sigint_handler(int sig, siginfo_t *info, void *ctx);
t_obj_arr	*ft_sh_init_builtin_ops(t_obj_arr **ops);
int			add_random_numbers_to_str(char *str_buf, int rand_count);
int			ft_wait_for_pid(int *wstatus, pid_t pid);
int			ft_wait_for_pid_stop(int *wstatus, pid_t pid);
int			ft_decode_wstatus(int wstatus);
int			ft_is_builtin(t_cmd_node *cmd, t_ctx *ctx);
void		ft_reset_sighandlers(t_ctx *ctx);
void		ft_set_signal_handlers(t_ctx *ctx);
int			ft_sh_init_term(t_ctx *ctx);
void		*global_ctx(void *val, t_get_or_set flag);
int			ft_shell_handle_redirect(t_wrd *wrd, int fd_redir,
				t_ctx *ctx, t_dir d);
void		remove_non_compliant_chars(char *buf, int buf_size);
void		ft_chdir_update_env_vars(t_ctx *ctx, char *oldpwd);
int			ft_handle_redirects(t_cmd_node *cmd, t_ctx *ctx);
int			ft_sh_is_legal_identifier(const char *name);
int			unlink_herefiles(t_ctx *ctx);
char		*heredoc_line_loop(char *l, t_hd_entry *en, t_ctx *ctx, int fd);
int			handle_parser_err(int errcode, t_lexer *lexer);
int			scan_the_line(const char *line, t_lexer *lexer, t_ctx *ctx);
t_token		*create_token(t_token_type type, const char *value, t_lexer *lexer);
void		flush_buffer(t_lexer *lexer, t_token_type type);
t_state		handle_initial(t_lexer *l, t_ctx *ctx);
t_state		handle_symbol(t_lexer *lexer, t_state state, t_ctx *ctx);
t_state		handle_in_single_quote(t_lexer *lexer);
t_state		exit_variable(t_lexer *l, t_ctx *ctx);
t_state		handle_variable(t_lexer *lexer, t_ctx *ctx);
t_state		handle_in_double_quote(t_lexer *lexer, t_ctx *ctx);
t_state		handle_check_append(t_lexer *lexer, int i, t_ctx *ctx);
t_state		handle_check_here_doc(t_lexer *lexer, int i, t_ctx *ctx);
t_state		handle_reading_whitespace(t_lexer *lexer, t_ctx *ctx);
char		*get_working_directory(t_ctx *ctx);
void		free_tokens(t_lexer *lexer);
void		parse_redirection(int *tp, t_ast_node *p, t_ctx *ctx, t_lexer *l);
t_ast_node	*parse_command(t_token **t, int *tp, t_ctx *ctx, t_lexer *l);
int			ft_do_parse(const char *line, t_ast_node **root, t_ctx *ctx);
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
void		ft_print_arg_node(t_wrd *arguments);
void		end_of_heredoc_check(t_lexer *lexer);
void		ft_heredoc_file_lexing(int fd, char *line, bool quotes, t_ctx *ctx);
int			herefile_varname(int i, char *var, char *line);
void		herefile_expansion(int fd, const char *varname, t_ctx *ctx);
t_state		create_pid_token(t_lexer *lexer, t_ctx *ctx);
int			ft_sh_give_terminal_to(pid_t pgrp);
pid_t		ft_tcgetpgrp(int fd);
int			ft_tcsetpgrp(int fd, pid_t pgrp);
int			here_doc_cat(t_wrd *here, t_lexer *l);
char		*hd_cat_loop(t_wrd *here, size_t len, t_lexer *l);
int			ft_sh_collect_heredocs(t_ctx *ctx);
t_state		handle_1(t_lexer *lexer);
t_state		handle_2(t_lexer *lexer, t_ctx *ctx);
t_state		scan_loop(t_lexer *l, t_ctx *ctx);
int			ft_collect_heredocs_loop(t_ctx *ctx);
int			ft_getpid_c(void);
int			create_here_file(t_wrd *here, t_hd_entry *entry, bool expand);

void		ft_cleanup_argv(t_ctx *ctx);
int			ft_sh_execute_command(t_ast_node *cmd, int level);
int			exec_disc_command(t_cmd_node *cmd, t_ctx *ctx);
void		ft_sh_sig_block(sigset_t *oldset);
void		ft_exec_with_sig_block(t_cmd_node *cmd, t_ctx *ctx);
void		ft_sh_run_forked(t_ast_node *cmd,
				int level, const int *fd, int fd_idx);
int			ft_run_builtin(t_cmd_node *cmd, t_ctx *ctx);
void		ft_sh_run_leader(t_ast_node *cmd, int level, int status);
int			ft_sh_wait_leader(const t_ast_node *cmd, pid_t fork_pid);
void		ft_sh_init_welcome(void);
int			ft_sh_loop(t_ctx *ctx);

/* Utils */
char		*ft_get_word(t_wrd *wrd, t_ctx *ctx);
char		**ft_get_argv(t_cmd_node *cmd, int *size, t_ctx *ctx);
int			ft_sh_lookup_pathname(t_ctx *ctx, t_cmd_node *cmd);

enum
{
	MAXC = 128
};

/**
 * k => key
 * v => value
 * attrs => attributes
 */
typedef struct s_shell_var
{
	const char	*k;
	char		*v;
	int			attrs;
}	t_sh_var;

# define RL_GREEN "\001\e[0;32m\002"
# define RL_BLUE "\001\e[:;34m\002"
# define RL_RESET "\001\e[0;m\002"

/* ---------- TESTS -------------------- */

void		check(bool succes);
void		ft_print_title(char *title);
typedef int						(*t_shell_fun)(t_ctx *);

typedef struct s_shell_op
{
	const char	*inst;
	t_shell_fun	fun;
}	t_shell_op;

/* ---------- FUNCTIONS -------------------- */
int			ft_chdir(t_ctx *ctx);
int			ft_env(t_ctx *ctx);
int			ft_pwd(t_ctx *ctx);
int			ft_echo(t_ctx *ctx);
int			ft_export(t_ctx *ctx);
int			ft_exit_with_exitcode(t_ctx *ctx);
int			ft_unset(t_ctx *ctx);

/* ---------- UTILS -------------------- */

void		ft_perrorf(const char *fmt, ...);
char		*ft_sh_read_line(t_ctx *ctx, t_prompt_type type);
int			ft_sh_init_interactive(t_ctx **ctx, char **envp);
int			ft_sh_init_noninteractive(t_ctx **ctx, char **envp);
int			ft_sh_op_cmp(const void *a, const void *b);
int			ft_sh_var_cmp(const void *a, const void *b);
int			ft_sh_parse_env_map(t_obj_arr *map, char **env_tab);
char		**ft_sh_render_envp(t_ctx *ctx);
int			ft_run_disc_command(t_cmd_node *cmd, t_ctx *ctx);
int			ft_sh_env_map_del_entry(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_add_entry(t_sh_var var, t_ctx *ctx);
t_sh_var	*ft_sh_env_map_get_entry(const char *key, t_ctx *ctx);
char		*ft_sh_env_map_get_val(const char *key, t_ctx *ctx);
int			ft_sh_env_map_get_index(const char *key, t_ctx *ctx);
int			ft_sh_env_map_bind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_unbind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_destroy_ctx(t_ctx *ctx);

#endif //MINISHELL_H
