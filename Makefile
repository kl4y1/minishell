CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iinclude -I.

NAME    = minishell

SRC     = minishellmain.c minishellmain2.c minishellmain3.c \
		parser/parser.c parser/parser2.c parser/parser_validate.c parser/parser_validate2.c \
		promptandsigs/ctrl.c promptandsigs/fresh.c \
		genutils/errors.c genutils/utils.c genutils/utils2.c genutils/utils3.c \
		excution/exec_utils.c excution/pipework.c excution/pipework_utils.c \
		excution/pipework_utils2.c excution/redir.c excution/paths.c \
		excution/heredoc.c excution/heredocutils.c \
		tokinizer/tokenizer.c tokinizer/tokenizer2.c tokinizer/tokenizer3.c \
		env/env.c env/env_utils.c \
		expander/expander.c expander/expander_utils.c \
		expander/expander_utils2.c expander/expander_utils3.c \
		expander/expander_utils4.c \
		builtins/b_utils.c builtins/b_utils2.c builtins/b_utils3.c builtins/b_utils4.c \
		builtins/cd.c builtins/echo.c builtins/env.c \
		builtins/exit.c builtins/export.c builtins/export2.c builtins/pwd.c builtins/unset.c

OBJ     = $(SRC:.c=.o)

LIBFT   = libft/libft.a
HEADERS = include/minishell.h

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Compiling Minishell..."
	@$(CC) $(CFLAGS) $(OBJ) -lreadline -lhistory -lncurses $(LIBFT) -o $(NAME)
	@echo "Minishell compiled successfully!"

$(LIBFT):
	@echo "Compiling libft..."
	@$(MAKE) -C libft > /dev/null
	@echo "libft compiled successfully!"

%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ)
	@$(MAKE) -C libft clean > /dev/null

fclean: clean
	@echo "Removing executable..."
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean > /dev/null

re: fclean
	@$(MAKE) all

.PHONY: all clean fclean re
