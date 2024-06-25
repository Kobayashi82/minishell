# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/16 11:45:10 by vzurera-          #+#    #+#              #
#    Updated: 2024/06/25 13:04:33 by vzurera-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ───────────────────────────────────────────────────────────── #
# ─────────────────────── CONFIGURATION ─────────────────────── #
# ───────────────────────────────────────────────────────────── #

# ──────────── #
# ── COLORS ── #
# ──────────── #

NC    				= \033[0m
RED     			= \033[0;31m
GREEN   			= \033[0;32m
YELLOW  			= \033[0;33m
CYAN    			= \033[0;36m
WHITE   			= \033[0;37m
INV_CYAN			= \033[7;36m
BG_CYAN				= \033[40m
FG_YELLOW			= \033[89m
COUNTER 			= 0

# ─────────── #
# ── FLAGS ── #
# ─────────── #

FLAGS				= -Wall -Wextra -Werror # -fsanitize=address
EXTRA_FLAGS			= -L${HOME}/.brew/opt/readline/lib -lreadline
EXTRA_FLAGS_OBJ 	= -I${HOME}/.brew/opt/readline/include
# EXTRA_FLAGS			= -lreadline

# ─────────────── #
# ── VARIABLES ── #
# ─────────────── #

ENABLE_LIBFT		= 1
ENABLE_NORMINETTE	= 1

# ────────── #
# ── NAME ── #
# ────────── #

NAME				= minishell

# ─────────── #
# ── FILES ── #
# ─────────── #

SRCS	=	terminal/banner.c terminal/prompt.c terminal/quotes.c				\
			terminal/input.c terminal/sub_input.c terminal/last_cmd.c			\
			terminal/escapes_1.c terminal/escapes_2.c terminal/escapes_3.c 		\
																				\
			lexer/lexer.c lexer/tokens.c lexer/parser.c lexer/separators.c		\
			lexer/args_list.c lexer/redirects.c lexer/redir.c lexer/syntax.c	\
																				\
			variables/shell_var.c variables/expand.c variables/expand_quotes.c	\
			variables/assign.c variables/validate.c variables/variables.c		\
			variables/environment.c variables/shell.c variables/extra.c			\
																				\
			wildcards/wildcards.c wildcards/pattern.c wildcards/brackets.c		\
																				\
			builtsin/builtsin.c builtsin/echo.c builtsin/cd.c builtsin/pwd.c	\
			builtsin/export_1.c builtsin/export_2.c builtsin/export_3.c 		\
			builtsin/env.c builtsin/unset.c builtsin/exit.c builtsin/about.c	\
			builtsin/history_1.c builtsin/history_2.c builtsin/help.c			\
																				\
			executer/pre_executer.c executer/executer.c executer/subshell_par.c	\
			executer/subshell_var.c executer/forked.c executer/forked_subshell.c\
			executer/redirections.c executer/heredoc.c executer/herestring.c	\
			executer/expand_redirection.c executer/buildpath.c					\
																				\
			clean/safe.c clean/free.c clean/errors.c							\
																				\
			main/initialize.c main/signals.c main/main.c

# ───────────────── #
# ── DIRECTORIES ── #
# ───────────────── #

INC_DIR				= ./inc/
OBJ_DIR				= ./build/obj/
LIB_DIR				= ./build/lib/
LIBFT_INC			= ./src/libft/inc/
LIBFT_DIR			= ./src/libft/
LIBFT				= libft.a
SRC_DIR				= ./src/$(NAME)/

# ────────────────────────────────────────────────────────── #
# ───────────────────────── NORMAL ───────────────────────── #
# ────────────────────────────────────────────────────────── #

TARGET=$(if $(SRCS),$(NAME),empty)
TARGET=$(if $(NAME),$(NAME),empty)
all: $(TARGET)

empty:
#	Check if NAME is empty
	@rm -f .is_re; if [ ! -n "$(NAME)" ] || [ ! -n "$(SRCS)" ]; then printf "\n\t$(CYAN)source files doesn't exist\n\n$(NC)"; exit 1; fi

#all: $(NAME)

OBJS		= $(SRCS:%.c=$(OBJ_DIR)%.o)
DEPS		= $(OBJS:.o=.d)
-include $(DEPS)

$(NAME): normal_extra $(OBJS)
#	Compile program
	@if [ -f $(NAME) ]; then \
		printf "\r%50s\r\t$(CYAN)Compiled    $(GREEN)✓ $(YELLOW)$(NAME)$(NC)"; \
	else \
		printf "\r%50s\r\t$(CYAN)Compiling... $(YELLOW)$(NAME)$(NC)"; \
	fi
	@if [ "$(ENABLE_LIBFT)" = "1" ] && [ -d "$(LIBFT_DIR)" ]; then \
        gcc $(FLAGS) -I$(INC_DIR) $(OBJS) $(LIB_DIR)$(LIBFT) -o $(NAME) $(EXTRA_FLAGS); \
    else \
        gcc $(FLAGS) -I$(INC_DIR) $(OBJS) -o $(NAME) $(EXTRA_FLAGS); \
    fi
	@printf "\r%50s\r\t$(CYAN)Compiled    $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"
#	Progress line
	@$(MAKE) -s progress
#	Norminette
	@if [ "$(ENABLE_NORMINETTE)" = "1" ]; then \
		printf "\n\t$(WHITE)─────────────────────────$(NC)\033[1A\r"; \
		printf "\r%50s\r\t$(CYAN)Norminette  $(YELLOW)scanning...$(NC)"; \
		output2=""; \
		if [ "$(ENABLE_LIBFT)" = "1" ] && [ -d "$(LIBFT_DIR)" ]; then output2=$$(norminette $(LIBFT_DIR) 2>&1); fi; \
		output1=$$(norminette $(SRC_DIR) 2>&1); \
		if echo $$output1 $$output2 | grep -q "Error"; then \
			printf "\r%50s\r\t$(CYAN)Norminette  $(RED)X $(YELLOW)errors$(NC)\n"; \
		else \
			printf "\r%50s\r\t$(CYAN)Norminette  $(GREEN)✓ $(YELLOW)perfect$(NC)\n"; \
		fi; $(MAKE) -s progress; \
	fi; printf "\n"
#	Restore cursor
	@$(MAKE) -s show_cursor
	@touch .first_start

# ───────────── #
# ── OBJECTS ── #
# ───────────── #

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
#	Create folder
	@mkdir -p $(@D)
#	Compile objects
	@filename=$$(basename $<); filename=$${filename%.*}; \
	BAR=$$(printf "/ ─ \\ |" | cut -d" " -f$$(($(COUNTER) % 4 + 1))); \
	printf "\r%50s\r\t$(CYAN)Compiling... $(GREEN)$$BAR  $(YELLOW)$$filename$(NC)"; \
	$(eval COUNTER=$(shell echo $$(($(COUNTER)+1))))
	@gcc $(FLAGS) -I$(INC_DIR) -I$(LIBFT_INC) -MMD -o $@ -c $(EXTRA_FLAGS_OBJ) $<

# ───────────────── #
# ── EXTRA RULES ── #
# ───────────────── #

normal_extra:
#	Check if NAME is empty and source directory exists
	@if [ ! -n "$(NAME)" ] || [ ! -n "$(SRCS)" ] || [ ! -d "$(SRC_DIR)" ]; then printf "\n\t$(CYAN)source files doesn't exist\n\n$(NC)"; rm -f .is_re; exit 1; fi
#	Hide cursor
	@$(MAKE) -s hide_cursor
#	Create folders
	@mkdir -p build/lib
#	Title
	@if [ ! -f .is_re ]; then clear; $(MAKE) -s title; fi; rm -f .is_re
#	Compile LIBFT
	@if [ "$(ENABLE_LIBFT)" = "1" ] && [ -d "$(LIBFT_DIR)" ]; then \
		(make -s all -C $(LIBFT_DIR)/; exit 0); $(MAKE) -s hide_cursor; \
	else \
		printf "\t$(WHITE)─────────────────────────\n$(NC)"; \
	fi; printf "\n\t─────────────────────────$(NC)\033[1A\r"

# ───────────────────────────────────────────────────────────── #
# ────────────────────────── RE-MAKE ────────────────────────── #
# ───────────────────────────────────────────────────────────── #

re:
#	Check if NAME is empty and source directory exists
	@rm -f .is_re; if [ ! -n "$(NAME)" ] || [ ! -n "$(SRCS)" ] || [ ! -d "$(SRC_DIR)" ]; then printf "\n\t$(CYAN)source files doesn't exist\n\n$(NC)"; exit 1; fi
#	Hide cursor
	@$(MAKE) -s hide_cursor
#	FClean
	@$(MAKE) -s fclean
#	Create files
	@touch .is_re; printf "\033[1A\033[1A\r"
#	Execute $(NAME)
	@$(MAKE) -s $(NAME)

# ───────────────────────────────────────────────────────────── #
# ─────────────────────────── CLEAN ─────────────────────────── #
# ───────────────────────────────────────────────────────────── #

clean:
#	Check if NAME is empty
	@rm -f .is_re; if [ ! -n "$(NAME)" ] || [ ! -n "$(SRCS)" ]; then printf "\n\t$(CYAN)source files doesn't exist\n\n$(NC)"; exit 1; fi
#	Hide cursor
	@$(MAKE) -s hide_cursor
#	Title
	@clear; $(MAKE) -s title
#	Delete objects
	@if [ "$(ENABLE_LIBFT)" = "1" ] && [ -d "$(LIBFT_DIR)" ]; then \
		(make -s delete_objects -C $(LIBFT_DIR)/; exit 0); \
	else \
		printf "\t$(WHITE)─────────────────────────\n$(NC)"; \
	fi
#	Delete objects
	@$(MAKE) -s delete_objects 
	@printf "\r%50s\r\t$(CYAN)Deleted     $(GREEN)✓ $(YELLOW)objects$(NC)\n"
#	Progress line
	@$(MAKE) -s progress; printf "\n"
#	Restore cursor
	@$(MAKE) -s show_cursor

# ───────────────────────────────────────────────────────────── #
# ────────────────────────── F-CLEAN ────────────────────────── #
# ───────────────────────────────────────────────────────────── #

fclean:
#	Check if NAME is empty
	@rm -f .is_re; if [ ! -n "$(NAME)" ] || [ ! -n "$(SRCS)" ]; then printf "\n\t$(CYAN)source files doesn't exist\n\n$(NC)"; exit 1; fi
#	Hide cursor
	@$(MAKE) -s hide_cursor
#	Title
	@clear; $(MAKE) -s title
#	Delete LIBFT
	@if [ "$(ENABLE_LIBFT)" = "1" ] && [ -d "$(LIBFT_DIR)" ]; then \
		(make -s -C $(LIBFT_DIR) fclean; exit 0); \
	else \
		printf "\t$(WHITE)─────────────────────────\n$(NC)"; \
	fi
#	Delete objects
	@$(MAKE) -s delete_objects
#	Delete $(NAME)
	@if [ -f $(NAME) ]; then \
		printf "\t$(CYAN)Deleting... $(YELLOW)$(NAME)$(NC)"; \
		rm -f $(NAME); \
	fi
	@printf "\r%50s\r\t$(CYAN)Deleted     $(GREEN)✓ $(YELLOW)$(NAME)$(NC)\n"
	@$(MAKE) -s progress; printf "\n"
#	Delete folder and files
	@-rm -d $(LIB_DIR) >/dev/null 2>&1 || true
	@-rm -d ./build >/dev/null 2>&1 || true
#	Restore cursor
	@$(MAKE) -s show_cursor
	@-rm -f .first_start

# ───────────────────────────────────────────────────────────── #
# ───────────────────────── FUNCTIONS ───────────────────────── #
# ───────────────────────────────────────────────────────────── #

# ─────────── #
# ── TITLE ── #
# ─────────── #

title:
	@printf "\n$(NC)\t$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★\
	$(INV_CYAN)  $(NC)$(INV_CYAN)$(shell echo $(NAME) | tr a-z A-Z | tr '_' ' ')$(INV_CYAN)  \
	$(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(BG_CYAN)$(FG_YELLOW)★$(INV_CYAN) $(NC)\n"

# ───────────── #
# ── CURSORS ── #
# ───────────── #

hide_cursor:
	@printf "\e[?25l"
 
show_cursor:
	@printf "\e[?25h"

# ──────────────────── #
# ── DELETE OBJECTS ── #
# ──────────────────── #

delete_objects:
	@printf "\n\t$(WHITE)─────────────────────────$(NC)\033[1A\r"
	@COUNTER=0; \
	for src in $(SRCS); do \
		srcname=$$(basename $$src .c); \
		file=$$(find $(OBJ_DIR) -name "$$srcname.o" 2>/dev/null); \
		if [ -n "$$file" ]; then \
			BAR=$$(printf "/ ─ \\ |" | cut -d" " -f$$((COUNTER % 4 + 1))); \
			rm -f $$file $$(dirname $$file)/$$srcname.d 2>/dev/null; \
			printf "\r%50s\r\t$(CYAN)Deleting... $(GREEN)$$BAR $(YELLOW)$$srcname$(NC)"; sleep 0.05; \
			COUNTER=$$((COUNTER+1)); \
		fi; \
	done; \
	printf "\r%50s\r"
	@-find $(OBJ_DIR) -type d -empty -delete >/dev/null 2>&1 || true
	@-rm -rf build ; rm -f .is_re $(NAME)

wipe:
	@rm -rf build ; rm -f .is_re $(NAME) ; rm -f .first_start

# ─────────────────── #
# ── PROGRESS LINE ── #
# ─────────────────── #

progress:
	@total=25; printf "\r\t"; for i in $$(seq 1 $$total); do printf "$(RED)─"; sleep 0.01; done; printf "$(NC)"
	@total=25; printf "\r\t"; for i in $$(seq 1 $$total); do printf "─"; sleep 0.01; done; printf "\n$(NC)"

# ─────────── #
# ── PHONY ── #
# ─────────── #

.PHONY: all clean fclean re normal_extra wipe delete_objects title hide_cursor show_cursor progress
