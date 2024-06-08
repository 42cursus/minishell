# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile.inc                                       :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/16 00:38:57 by abelov            #+#    #+#              #
#    Updated: 2023/12/16 00:39:02 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ft_printf sources

FILES   = ft_printf.c \
			ft_print_int.c \
			ft_print_str.c \
			ft_print_unsigned.c

SRC    += $(patsubst %.c,printf/%.c,$(FILES))