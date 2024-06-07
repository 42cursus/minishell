# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yublee <yublee@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/19 14:55:22 by yublee            #+#    #+#              #
#    Updated: 2024/05/05 12:00:24 by yublee           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIRS = src gnl

CC = cc

CFLAGS = -Wall -Wextra -Werror -g

RM = rm

RMFLAGS = -fv

AR = ar

ARFLAGS = rcs

INCLUDE = -I include/

NAME = pipex

SRC = src/main.c \
	src/pipex.c \
	src/utils.c \
	gnl/get_next_line.c \
	gnl/get_next_line_utils.c \

	
BONUS_SRC = src/main_bonus.c \
	src/pipex_bonus.c \
	src/utils_bonus.c \
	gnl/get_next_line.c \
	gnl/get_next_line_utils.c \

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
BONUS_OBJ = $(BONUS_SRC:%.c=$(BUILD_DIR)/%.o)

LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

BUILD_DIR = build

ifdef WITH_BONUS
	OBJ_FILES = $(BONUS_OBJ)
else
	OBJ_FILES = $(OBJ)
endif

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(BUILD_DIR) $(OBJ_FILES)
	$(CC) -L$(LIBFT_DIR) $(OBJ_FILES) -g -o $@ -lft

$(LIBFT_LIB):
	make -C $(LIBFT_DIR)

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR) $(patsubst %,$(BUILD_DIR)/%,$(DIRS))

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@

bonus: 
	make WITH_BONUS=1 all

clean:
	$(RM) -rf $(BUILD_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(RMFLAGS) $(NAME)
	$(RM) $(RMFLAGS) $(LIBFT_LIB)

re: fclean all

.PHONY: all clean fclean re bonus
