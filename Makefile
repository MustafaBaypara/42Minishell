NAME = minishell

CC = cc 

CFLAGS = -Wall -Wextra -Werror -g

LIB = srcs/libft

CFILES = srcs/minishell.c srcs/tools/garbagecollector.c srcs/loop.c \
        srcs/utils/utils.c srcs/utils/utils2.c srcs/lexer/lexer.c srcs/lexer/syntax.c \
		srcs/parser/parser.c srcs/parser/parser_utils.c srcs/parser/parser_utils2.c \
		srcs/utils/heredoc.c srcs/utils/utils3.c srcs/utils/signals.c srcs/expander/expander.c \
		srcs/expander/expander_utils.c srcs/executor/executor.c srcs/executor/executor_utils.c \
		srcs/builtin/builtin.c srcs/executor/executor_utils2.c srcs/executor/executor_utils3.c \


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
