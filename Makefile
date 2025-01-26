# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/12 00:27:48 by abelov            #+#    #+#              #
#    Updated: 2024/12/12 00:27:49 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= minishell
CC				:= cc
INC_DIR			=  ./include
INCLUDE_FLAGS	:= -I. -I$(INC_DIR) -I/usr/include
OPTIMIZE_FLAGS	:= -O0
DEBUG_FLAGS		:= -g3 -gdwarf-3 # -fsanitize=address -fsanitize=undefined
MANDATORY_FLAGS	:= -Wall -Wextra -Werror -Wimplicit
CFLAGS			= $(MANDATORY_FLAGS) $(DEBUG_FLAGS) $(OPTIMIZE_FLAGS) $(INCLUDE_FLAGS)

LIBFT_DIR		=  ./lib/ft
LIBFT_LIB		=  $(LIBFT_DIR)/libft.a
LIBS			:= $(LIBFT)
LINK_FLAGS		:= -L $(LIBFT_DIR) -lft -L/usr/lib/x86_64-linux-gnu -lreadline

SRC_DIR			= src

SH_DIRS			= builtins error exec parser utils
SH_SRCS	 		:=

include $(SH_DIRS:%=$(SRC_DIR)/%/Makefile.mk)

SRCS			:= src/main.c
SRCS			+= $(SH_SRCS)

$(info =================)
$(info )
$(info $(SH_DIRS))
$(info )
$(info =================)
$(info =================)
$(info )
$(info $(SH_SRCS))
$(info )
$(info =================)

BUILD_DIR		= build
OBJS			= $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJS)
		$(CC) $(OBJS) $(DEBUG_FLAGS) -fsanitize=address -fsanitize=undefined -o $@ $(LINK_FLAGS)

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
