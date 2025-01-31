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
DEBUG_FLAGS		:= -g3 -gdwarf-3 -fsanitize=address -fsanitize=undefined
MANDATORY_FLAGS	:= -Wall -Wextra -Werror -Wimplicit
CFLAGS			= $(MANDATORY_FLAGS) $(DEBUG_FLAGS) $(OPTIMIZE_FLAGS) $(INCLUDE_FLAGS)

LIBFT_DIR		=  ./lib/ft
LIBFT_LIB		=  $(LIBFT_DIR)/libft.a
LIBS			:= $(LIBFT)
LINK_FLAGS		:= -L $(LIBFT_DIR) -lft -L/usr/lib/x86_64-linux-gnu \
 					-lreadline -fsanitize=address -fsanitize=undefined

SRC_DIR			= src

SH_DIRS			= builtins error exec parser utils
SH_SRCS	 		:=

ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif

include $(SH_DIRS:%=$(SRC_DIR)/%/Makefile.mk)

SRCS			:= src/main.c
SRCS			+= $(SH_SRCS)

ifdef PRINT_SOURCES

# $(NULL) is empty string
NULL  :=
# $(SPACE) is one space
SPACE := $(NULL) $(NULL)
# $(\n) is new line
define \n


endef

$(info =================$(\n))
$(info $SH_SRCS: $(subst $(SPACE),$(\n)    ,$(SH_SRCS))$(\n))
$(info you can compare this output with )
$(info $(\n)    find src -name '*.c' | sort $(\n))
$(info =================$(\n))

endif

BUILD_DIR		= build
OBJS			= $(SRCS:%.c=$(BUILD_DIR)/%.o)

## all
all: $(NAME)

## minishell
$(NAME): $(LIBFT_LIB) $(OBJS)
		@$(CC) $(OBJS) $(DEBUG_FLAGS) -o $@ $(LINK_FLAGS)
		@echo "MINISHELL BUILD COMPLETE!"

## libft
$(LIBFT_LIB):
		@$(MAKE) -C $(LIBFT_DIR) --no-print-directory -j8

$(BUILD_DIR)/%.o: %.c
		@if [ ! -d $(@D) ]; then mkdir -p $(@D); fi
		@$(CC) $(CFLAGS) $(INCLUDE) -c $^ -o $@

## clean
clean:
		@$(RM) -rf $(BUILD_DIR)
		@$(MAKE) -C $(LIBFT_DIR) clean

## fclean
fclean: clean
		@$(RM) $(RMFLAGS) $(NAME)
		@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

norm:
		@norminette $(SRCS)
		@$(MAKE) -C $(LIBFT_DIR) norm

# Magic help adapted: from https://gitlab.com/depressiveRobot/make-help/blob/master/help.mk (MIT License)
help:
	@printf "Available targets:\n\n"
	@awk -F: '/^[a-zA-Z\-_0-9%\\ ]+:/ { \
			helpMessage = match(lastLine, /^## (.*)/); \
			if (helpMessage) { \
					helpCommand = $$1; \
					helpMessage = substr(lastLine, RSTART + 3, RLENGTH); \
					printf "  \x1b[32;01m%-35s\x1b[0m %s\n", helpCommand, helpMessage; \
			} \
	} \
	{ lastLine = $$0 }' $(MAKEFILE_LIST) | sort -u
	@printf "\n"

.PHONY: all clean fclean re bonus norm help
