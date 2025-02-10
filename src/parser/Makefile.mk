# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 17:04:40 by abelov            #+#    #+#              #
#    Updated: 2025/01/26 17:04:41 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES   = ft_parse_line.c \
			lexer.c \
			parser.c \
            ft_heredoc_file_lexing.c \
			parse_print.c \
			lexer_states.c \
			lexer_states_utils.c \
			lexer_utils.c \
			parse_hd_delimiter.c

SH_SRCS    += $(FILES:%.c=$(dir $(lastword $(MAKEFILE_LIST)))%.c)
