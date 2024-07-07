/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 18:09:58 by abelov            #+#    #+#             */
/*   Updated: 2024/06/28 18:09:59 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdbool.h>
# include <stdio.h>
# include <signal.h>
# include <sysexits.h>
# include <asm-generic/errno.h>
# include <linux/limits.h>
# include "libft.h"

/**
 * k => key
 * v => value
 * attrs => attributes
 */
typedef struct s_shell_var
{
	const char	*k;
	char		*v;
	int 		attrs;
}	t_sh_var;

typedef struct s_exec_ctx
{
	char		**argv;
	int 		argc;
	int 		flags;
	struct s_inout
	{
		int in;
		int out;
		int err;
	}			fdio;
	t_obj_arr	env_map;
}	t_exec_ctx;

# define ATT_EXPORTED	0x0000001	/* export to environment */
# define ATT_READONLY	0x0000002	/* cannot change */
# define ATT_INTEGER	0x0000010	/* internal representation is int */
# define ATT_NOUNSET 	0x0002000	/* cannot unset */
# define ATT_NOASSIGN	0x0004000	/* assignment not allowed */
# define ATT_IMPORTED	0x0008000	/* came from environment */

# define FT_RED   "\033[0;31m"
# define FT_GREEN "\033[0;32m"
# define FT_CYAN  "\033[36m"
# define FT_RESET "\e[0m"

/* ---------- TESTS -------------------- */

void		check(bool succes);
void		ft_print_title(char *title);

typedef int	(*t_shell_fun)(t_exec_ctx *);
typedef struct s_shell_op
{
	const char	*instruction;
	t_shell_fun	fun;
}	t_shell_op;

/* ---------- FUNCTIONS -------------------- */
int			ft_chdir(t_exec_ctx *ctx);
int			ft_env(t_exec_ctx *ctx);
int			ft_pwd(t_exec_ctx *ctx);
int			ft_echo(t_exec_ctx *ctx);
int			ft_export(t_exec_ctx *ctx);
int			ft_unset(t_exec_ctx *ctx);

/* ---------- UTILS -------------------- */
void		sigsegv(int signal);
int			do_init_ops(t_obj_arr **ops);
int			do_init(t_exec_ctx	**ctx, char **envp);
int			ft_shell_op_cmp(const void *a, const void *b);
int			ft_shell_var_cmp(const void *a, const void *b);
int			ft_shell_parse_env_map(t_obj_arr *map, char **env_tab);
t_sh_var	*ft_shell_env_map_get_entry(const char *key, t_exec_ctx *ctx);
int			ft_shell_env_map_bind_var(t_sh_var var, t_exec_ctx *ctx);
int			ft_shell_env_map_unbind_var(t_sh_var var, t_exec_ctx *ctx);
#endif //TEST_H
