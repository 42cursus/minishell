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

FILES   = ft_ast_walk.c \
			ft_sh_launch.c \
			ft_shell_redirect.c \
			ft_sh_collect_heredocs.c \
			ft_heredoc_file_lexing.c

SH_SRCS    += $(FILES:%.c=$(dir $(lastword $(MAKEFILE_LIST)))%.c)
