/* SPDX-License-Identifier: BSD-3-Clause */


#ifndef __PARSER_H
#define __PARSER_H

#include <stdbool.h>
#include <linux/limits.h>
#include "ft/ft_stdlib.h"

#define SHELL_EXIT -100
#define IO_REGULAR	0x00
#define IO_OUT_APPEND	0x01
#define IO_ERR_APPEND	0x02

typedef struct s_wrd t_wrd;
struct s_wrd
{
	const char		*value;
	unsigned char	expand;
	unsigned char	append;
	t_wrd			*next_part;
	t_wrd			*next_word;
};


typedef struct word_t word_t;
struct word_t
{
	const char *string;
	bool expand;
	struct word_t *next_part;
	struct word_t *next_word;
};


/*
 * Describes a simple command

 * aux can be used to point to additional information you might need;
 * freeing the memory aux points to is your responsibility. aux will
 * be initialized to NULL. Do not modify these structures, please use
 * aux.

 * verb points to a single string literal (possibly made up of parts)
 * that is the executable name or the internal command name.

 * params points to a list of parameters (possibly none) in the order
 * they were entered in the command line.

 * in out and err point to the names of the redirections for the command;
 * they can point to a single literal, no literal (the command does not
 * have that redirection) or multiple literals (the user entered multiple
 * redirections for a command; e.g. cat >out1 >out2). You can handle the
 * latter as you wish (e.g. only consider the first redirection). Within
 * any of these lists, the literals are in the original order.

 * io_flags is used to specify special modes for redirection (e.g. appending)

 * Some string literals can be found in both the out list and the err list
 * (those entered as "command &> out").

 * up points to the command_t structure that points to this simple_command_t
 * (up != NULL)
 */

#define IO_REGULAR	0x00
#define IO_OUT_APPEND	0x01
#define IO_ERR_APPEND	0x02

#define HEREDOC_ARRAY_SIZE 1024
#define FILENAME_BUF_SIZE 256


typedef enum e_errcodes
{
	OK = 0,
	COULDNT_OPEN_URANDOM ,
	UNCLOSED_QUOTE ,
	TOKEN_ALLOC_FAILURE ,
	PID_ALLOC_FAILURE ,
	ALLOC_FAILURE ,
	NO_REDIR_TARGET ,
	HD_CAT_FAILURE ,
} t_errcodes;

typedef struct HeredocEntry
{
	char delimiter[FILENAME_BUF_SIZE];
	char filename[FILENAME_BUF_SIZE];
	bool quotes;
}	HeredocEntry;

typedef struct HereArray
{
	HeredocEntry entries[HEREDOC_ARRAY_SIZE];
	int size;
	int ss;
}	HereArray;

typedef struct s_exec_ctx
{
	char		**argv;
	int 		argc;
	t_obj_arr	env_map;
	char 		pathname[PATH_MAX];
	t_obj_arr	*ops;
	HereArray	hd;
}	t_ctx;

typedef struct simple_cmd_s simple_cmd_t;
struct simple_cmd_s
{
	word_t *verb;
	word_t *params;
	word_t *in;
	word_t *out;
	word_t *err;
	int io_flags;
	struct command_s *up;
	t_ctx *ctx;
};


/*
 * Operators
 * OP_NONE means no operator
 * (the scmd field points to a simple command and cmd1 == cmd2 == NULL)
 * The rest of the operators mean scmd == NULL
 * OP_DUMMY is a dummy value that can be used to count the number of operators
 */
typedef enum e_op
{
	OP_SIMPLE,
	OP_SEQUENTIAL,
	OP_PARALLEL,
	OP_CONDITIONAL_ZERO,
	OP_CONDITIONAL_NZERO,
	OP_PIPE,
	OP_DUMMY
}	operator_t;

typedef enum e_node_type {
	NODE_COMMAND,
	NODE_PIPE,
} t_node_type;


typedef struct s_cmd_node	t_cmd_node;
struct s_cmd_node
{
	t_wrd	*args;

	t_wrd	*redirects_in;
	t_wrd	*redirects_err_in;
	t_wrd	*redirects_err;
	t_wrd	*redirects_out;
	t_ctx	*ctx;
};

typedef struct s_ast_node	t_ast_node;
struct s_ast_node
{
	t_ast_node	*parent;
	t_ast_node	*left;
	t_ast_node	*right;
	t_node_type	type;
	t_cmd_node	*cmd;
	t_ctx		*ctx;
};

typedef struct command_s cmd_t;
struct command_s {
	cmd_t			*up;
	cmd_t			*left;
	cmd_t			*right;
	operator_t		op;
	simple_cmd_t	*simple_cmd;
	t_ctx			*ctx;
	t_ast_node		*ast;
};

/*
 * You must define this function; it will be called by the parser
 * when a parse error occurs; str describes the error (not a useful
 * description) and where points to some location before the error
 * was discovered (the character number, starting at 0)
 */

void parse_error(const char *str, const int where);


/*
 * Call this to parse a line

 * line must point to a string containig a single line
 * The line must end with "\r\n\0" or "\n\0" or "\0"
 * (*root) must point to NULL ((*root) == NULL)

 * parse_line returns true if there was no error parsing the line
 * and false if there was an error parsing or the arguments were invalid

 * if parse_line returns true:
 *   if line was empty (or contained just blanks) (*root) is NULL
 *   else (*root) points to the root of the parse tree
 */

bool parse_line(const char *line, cmd_t **root);


/*
 * Should be called to free the parse tree
 * call this even if parse_line() returned false
 */

void free_parse_memory(void);

#ifdef __PARSER_H_INTERNAL_INCLUDE

/*
 * Parser and lexer common internal stuff
 */

typedef void *GenericPointer;

typedef struct {
	word_t *red_i;
	word_t *red_o;
	word_t *red_e;
	int red_flags;
} redirect_t;

void pointerToMallocMemory(const void *ptr);
int yylex(void);
void globalParseAnotherString(const char *str);
void globalEndParsing(void);

#endif

/*
 * Parser written by Maximilian Machedon, based on a previous version.
 */

#endif
