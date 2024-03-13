# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/06 09:56:25 by mafurnic          #+#    #+#              #
#    Updated: 2024/03/13 12:55:45 by mafurnic         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project Name and Compiler Settings
NAME := minishell
COMPILER := cc
FLAGS := -Wall -Wextra -Werror   
CLEANUP_CMD := rm -rf

# Directory Paths
OBJECTS_PATH := obj/
SOURCES_PATH := src/
LIBFT_PATH := libft/

# Source and Object Files

SOURCE_FILES := main.c lexer/lexer.c lexer/lexer_utils.c parser/parser.c custom/custom_functions1.c custom/custom_functions2.c custom/custom.c \
				custom/export.c error/free_and_close.c execution/executor.c execution/executor_utils1.c \
				env/arr_to_linked_list.c execution/executor_utils2.c /signals/signals.c \

OBJECTS := $(SOURCE_FILES:%.c=$(OBJECTS_PATH)%.o)
LIBFT := $(LIBFT_PATH)libft.a

# Console Output Colors
COLOR_RED := \033[0;91m
COLOR_GREEN := \033[0;92m
COLOR_YELLOW := \033[0;93m
COLOR_BLUE := \033[0;94m
COLOR_WHITE := \033[0;97m

# Build the Project
all: $(NAME)

# Linking the Program
$(NAME): $(LIBFT) $(OBJECTS)
	@echo "$(COLOR_YELLOW)Building $(NAME)$(COLOR_WHITE)"
	@$(COMPILER) $(FLAGS) $(OBJECTS) -L$(LIBFT_PATH) -lft -lreadline -o $(NAME)
	@echo "$(COLOR_GREEN)$(NAME) compiled successfully.$(COLOR_WHITE)"
	 

# Compiling Source Files
$(OBJECTS_PATH)%.o: $(SOURCES_PATH)%.c
	@mkdir -p $(@D)
	@echo "$(COLOR_GREEN)Compiling:$(COLOR_WHITE) $<"
	@$(COMPILER) $(FLAGS) -c $< -o $@

# Compiling libft
$(LIBFT):
	@echo "$(COLOR_BLUE)Compiling libft$(COLOR_WHITE)"
	@$(MAKE) -C $(LIBFT_PATH)


# Cleaning Up Object Files
clean:
	@echo "$(COLOR_RED)Removing object files and directory...$(COLOR_WHITE)"
	@$(CLEANUP_CMD) $(OBJECTS)
	@$(CLEANUP_CMD) $(OBJECTS_PATH)
	@echo "$(COLOR_BLUE)Cleaning libft$(COLOR_WHITE)"
	@$(MAKE) -C $(LIBFT_PATH) clean
	@sleep 1
	@clear

# Full Clean-Up
fclean: clean
	@echo "$(COLOR_RED)Removing executable...$(COLOR_WHITE)"
	@$(CLEANUP_CMD) $(NAME)
	@echo "$(COLOR_BLUE)Full clean for libft$(COLOR_WHITE)"
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@sleep 1
	@clear

# Recompile the Projects
re: fclean all

# Phony Targets
.PHONY: all clean fclean re valgrind

 