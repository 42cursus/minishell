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

FILES   = ft_strcat.c \
			ft_strchr.c \
			ft_strcmp.c \
			ft_strdup.c \
			ft_striteri.c \
			ft_strjoin.c \
			ft_strlcat.c \
			ft_strlcpy.c \
			ft_strlen.c \
			ft_strmapi.c \
			ft_strncat.c \
			ft_strncmp.c \
			ft_strndup.c \
			ft_strnew.c \
			ft_strnstr.c \
			ft_strrchr.c \
			ft_strspn.c \
			ft_strtok.c \
			ft_strtrim.c \
			ft_substr.c


SRCS    += $(FILES:%.c=$(dir $(lastword $(MAKEFILE_LIST)))%.c)