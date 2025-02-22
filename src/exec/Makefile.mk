# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 17:04:28 by abelov            #+#    #+#              #
#    Updated: 2025/01/26 17:04:28 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES   = ft_collect_heredocs_loop.c \
			ft_run_builtin.c \
			ft_sh_collect_heredocs.c \
			ft_sh_exec_cmd.c \
			ft_sh_launch.c \
			ft_sh_run_forked.c \
			ft_shell_redirect.c

SH_SRCS    += $(FILES:%.c=$(dir $(lastword $(MAKEFILE_LIST)))%.c)
