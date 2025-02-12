# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 17:05:23 by abelov            #+#    #+#              #
#    Updated: 2025/01/26 17:05:24 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES   = ft_get_argv.c \
			ft_get_word.c \
			ft_perrorf.c \
			ft_print_ast.c \
			ft_sh_cmp.c \
			ft_sh_env_map.c \
			ft_sh_env_map_entry_utils.c \
			ft_sh_env_map_utils.c \
			ft_sh_finalise.c \
			ft_sh_give_terminal_to.c \
			ft_sh_init.c \
			ft_sh_init_interactive.c \
			ft_sh_init_term.c \
			ft_sh_lookup_pathname.c \
			ft_sh_read_line.c \
			ft_sh_render_envp.c \
			ft_tcgetpgrp.c \
			ft_tcsetpgrp.c

SH_SRCS    += $(FILES:%.c=$(dir $(lastword $(MAKEFILE_LIST)))%.c)
