# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yublee <yublee@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/18 14:44:19 by yublee            #+#    #+#              #
#    Updated: 2024/02/19 14:18:49 by yublee           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIRS = printf std bonus

CC = cc

CFLAGS = -Wall -Wextra -Werror

RM = rm

RMFLAGS = -fv

AR = ar

ARFLAGS = rcs

INCLUDE = -I.

NAME = libft.a

SRC := 
BONUS_SRC := 

include $(addsuffix /Makefile.mk,$(DIRS))

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
BOBJ = $(BONUS_SRC:%.c=$(BUILD_DIR)/%.o)

BUILD_DIR = build

all: $(NAME)

$(NAME): $(BUILD_DIR) $(OBJ) $(BOBJ)
	$(AR) $(ARFLAGS) $(NAME) $(OBJ) $(BOBJ)

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR) $(patsubst %,$(BUILD_DIR)/%,$(DIRS))

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@

clean:
	$(RM) -rf $(BUILD_DIR)
	
fclean: clean
	$(RM) $(RMFLAGS) $(NAME)

re: fclean all

.PHONY: all clean fclean re
