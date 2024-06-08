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

BONUS_FILES = ft_lstnew.c \
			ft_lstadd_front.c \
			ft_lstsize.c \
			ft_lstlast.c \
			ft_lstadd_back.c \
			ft_lstdelone.c \
			ft_lstclear.c \
			ft_lstiter.c \
			ft_lstmap.c

BONUS_SRC    += $(patsubst %.c,bonus/%.c,$(BONUS_FILES))