NAME = minishell

CC = cc 

CFLAGS = -Wall -Wextra -Werror -g

LIB = srcs/libft

CFILES = srcs/minishell.c srcs/tools/garbagecollector.c srcs/loop.c \
        srcs/utils/utils.c srcs/utils/utils2.c srcs/exec.c srcs/lexer/lexer.c

OBJECTS = $(CFILES:.c=.o)

all: $(NAME)

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): libft $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -L$(LIB) -lft -lreadline -o $(NAME)

libft:
	@make -C $(LIB)
	@make bonus -C $(LIB)

clean:
	@rm -f $(OBJECTS)
	@make clean -C $(LIB)

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIB)

re: fclean all

.PHONY: all clean fclean re
