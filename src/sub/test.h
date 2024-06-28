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

# ifndef T_UINT
#  define T_UINT
/**
 * When an expression contains operands of different built-in types,
 * and no explicit casts are present, the compiler uses built-in standard
 * conversions to convert one of the operands so that the types match.
 *
 * See also:
 *	 https://learn.microsoft.com =>
 *	 	=> /en-us/cpp/cpp/type-conversions-and-type-safety-modern-cpp
 */
typedef unsigned int	t_uint;
# endif


int		ft_pwd(void);

# define FT_RED   "\033[0;31m"
# define FT_GREEN "\033[0;32m"
# define FT_CYAN  "\033[36m"
# define FT_RESET "\e[0m"

/* ---------- TESTS -------------------- */

void	check(bool succes);
void	ft_print_title(char *title);

typedef int				(*t_fun)(void);
typedef struct s_ops
{
	size_t size;
	const t_fun *functions;
} t_ops;

/* ---------- UTILS -------------------- */
void	sigsegv(int signal);

#endif //TEST_H