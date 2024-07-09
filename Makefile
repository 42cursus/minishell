# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yublee <yublee@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/19 14:55:22 by yublee            #+#    #+#              #
#    Updated: 2024/07/09 15:11:02 by yublee           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= minishell
CC				:= cc
INC_DIR			=  ./include
INCLUDE_FLAGS	:= -I. -I$(INC_DIR) -I/usr/include
OPTIMIZE_FLAGS	:= -O0
DEBUG_FLAGS		:= -g3 -gdwarf-3
MANDATORY_FLAGS	:= -Wall -Wextra -Werror -Wimplicit
CFLAGS			= $(MANDATORY_FLAGS) $(DEBUG_FLAGS) $(OPTIMIZE_FLAGS) $(INCLUDE_FLAGS)

LIBFT_DIR		=  ./lib/ft
LIBFT_LIB		=  $(LIBFT_DIR)/libft.a
LIBS			:= $(LIBFT)
LINK_FLAGS		:= -L $(LIBFT_DIR) -lft

SRCS			= src/main.c \
                  	src/syntax_validation.c \
                  	src/parsing_pipe.c \
                  	src/parsing_redirection.c \
                  	src/parsing_redirection_utils.c \
                  	src/parsing_tree_to_lst.c \
                  	src/pipex_main.c \
                  	src/pipex_child_process.c \
                  	src/pipex_io.c \
                  	src/pipex_argv.c \
                  	src/utils_str.c \
                  	src/utils_tree.c \
                  	src/utils_lst.c \
                  	src/utils.c

BUILD_DIR		= build
OBJS			= $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJS)
		$(CC) -L$(LIBFT_DIR) $(OBJS) -g -o $@ -lft

$(LIBFT_LIB):
		@$(MAKE) -C $(LIBFT_DIR) -j8

$(BUILD_DIR)/%.o: %.c
		@if [ ! -d $(@D) ]; then mkdir -pv $(@D); fi
		$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@

clean:
		$(RM) -rf $(BUILD_DIR)
		@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
		$(RM) $(RMFLAGS) $(NAME)
		@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

norm:
		@norminette $(SRCS)
		@$(MAKE) -C $(LIBFT_DIR) norm

.PHONY: all clean fclean re bonus norm
