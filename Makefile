CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I.

NAME    = minishell

SRC     = minishellmain.c mini_parser.c ctrl.c fresh.c utils.c errors.c paths.c \
			pipework.c pipeworkutils.c tokenizer.c \

OBJ     = $(SRC:.c=.o)

LIBFT   = libft/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@echo "Compiling Minishell..."
	@$(CC) $(CFLAGS) $(OBJ) -lreadline -lncurses $(LIBFT) -o $(NAME)
	@echo "Minishell compiled successfully!"

$(LIBFT):
	@echo "Compiling libft..."
	@$(MAKE) -C libft > /dev/null
	@echo "libft compiled successfully!"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJ)
	@$(MAKE) -C libft clean > /dev/null

fclean: clean
	@echo "Removing executable..."
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean > /dev/null

re: fclean all

.PHONY: all clean fclean re
