# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/28 17:40:38 by abelov            #+#    #+#              #
#    Updated: 2024/06/28 17:40:39 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES   = ft_strchr.c \
			ft_strlcpy.c \
			ft_strlcat.c \
			ft_strrchr.c \
          	ft_strspn.c \
			ft_strtok.c

SRCS    += $(FILES:%.c=$(dir $(lastword $(MAKEFILE_LIST)))%.c)