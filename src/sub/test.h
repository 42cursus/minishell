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
# include <error.h>
# include <stdio.h>
# include <signal.h>
# include <sysexits.h>
# include <asm-generic/errno.h>
# include <linux/limits.h>
# include "libft.h"

# include <signal.h>
# include <readline.h>

enum  { VAR_MAX = 256 };

typedef struct sigaction	t_sigaction;

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
	struct s_inout
	{
		int in;
		int out;
		int err;
	}			fdio;
	t_obj_arr	env_map;
	char *const	*envp;
	char 		pathname[PATH_MAX];
}	t_ctx;

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
int			ft_exit(t_ctx *ctx);

/* ---------- UTILS -------------------- */
void		sigsegv(int signal);
int			ft_sh_do_init(t_ctx	**ctx, char **envp, t_obj_arr **ops);
int			ft_sh_op_cmp(const void *a, const void *b);
int			ft_sh_var_cmp(const void *a, const void *b);
int			ft_sh_parse_env_map(t_obj_arr *map, char **env_tab);
char		**ft_sh_render_envp(t_ctx *ctx);
int			ft_sh_env_map_del_entry(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_add_entry(t_sh_var var, t_ctx *ctx);
t_sh_var	*ft_sh_env_map_get_entry(const char *key, t_ctx *ctx);
char		*ft_sh_env_map_get_val(const char *key, t_ctx *ctx);
int			ft_sh_env_map_get_index(const char *key, t_ctx *ctx);
int			ft_sh_env_map_bind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_env_map_unbind_var(t_sh_var var, t_ctx *ctx);
int			ft_sh_destroy_ctx(t_ctx *ctx);
#endif //TEST_H
