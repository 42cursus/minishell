/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 00:09:31 by abelov            #+#    #+#             */
/*   Updated: 2025/01/21 00:09:31 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include <linux/limits.h>
# include "ft/ft_stdlib.h"
# include <signal.h>

# define IO_REGULAR	0x00
# define IO_OUT_APPEND	0x01
# define IO_ERR_APPEND	0x02

typedef struct s_wrd
{
	const char		*value;
	unsigned char	expand;
	int 			redir_flag;
	struct s_wrd	*next_part;
	struct s_wrd	*next_word;
}	t_wrd;

# define HEREDOC_ARRAY_SIZE 1024
# define FILENAME_BUF_SIZE 256

typedef enum e_errcodes
{
	OK = 0,
	UNEXPECTED_DUMMY,
	COULDNT_OPEN_URANDOM,
	UNCLOSED_QUOTE,
	TOKEN_ALLOC_FAILURE,
	PID_ALLOC_FAILURE,
	ALLOC_FAILURE,
	NO_REDIR_TARGET,
	OP_AT_BEGINNING, 
	HD_CAT_FAILURE,
	SINGLE_AMPERSAND,
	EMPTY_LINE
}	t_errcodes;

typedef struct s_hd_entry
{
	char	delimiter[FILENAME_BUF_SIZE];
	char	filename[FILENAME_BUF_SIZE];
	bool	quotes;
}	t_hd_entry;

typedef struct s_here_array
{
	t_hd_entry	entries[HEREDOC_ARRAY_SIZE];
	int			size;
	int			ss;
	int 		already_unlinked;
}	t_here_arr;

typedef struct sigaction	t_sigaction;

typedef enum e_prompt
{
	PS_REGULAR = 0,
	PS_HERE
}	t_prompt_type;

typedef struct s_exec_ctx
{
	char			**argv;
	char			**envp;
	int				argc;
	t_obj_arr		env_map;
	char			pathname[PATH_MAX];
	t_obj_arr		*ops;
	t_here_arr		hd;
	int				last_status_code;
	pid_t			parent_tpgrp;
	int				received_signal_num;
	t_sigaction		act[64];
	t_sigaction		old_act[64];
	const char		*ps0;
	const char		*ps1;
	const char		*ps2;
	t_prompt_type	prompt_type;
}	t_ctx;


typedef enum e_node_type
{
	NODE_DUMMY,
	NODE_COMMAND,
	NODE_PIPE,
	NODE_OR,
	NODE_AND,
}	t_node_type;

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

#endif
