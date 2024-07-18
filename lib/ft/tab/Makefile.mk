# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/01 18:15:26 by abelov            #+#    #+#              #
#    Updated: 2024/07/02 17:55:34 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES   = ft_tab_count_if.c \
			ft_tab_foreach.c \
			ft_tab_get_size.c \
			ft_tab_str_free.c \
			ft_tab_str_join.c

SRCS    += $(FILES:%.c=$(dir $(lastword $(MAKEFILE_LIST)))%.c)
