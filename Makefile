MAKEFLAGS 		+= --no-print-directory

SRCS_DIR		=	./srcs/
OBJS_DIR		=	./objs/

BUILTIN_DIR		=	$(SRCS_DIR)builtin/

EXECUTOR_DIR	=	$(SRCS_DIR)executor/

EXPANDER_DIR	=	$(SRCS_DIR)expander/

LEXER_DIR		=	$(SRCS_DIR)lexer/

PARSER_DIR		=	$(SRCS_DIR)parser/

TOOLS_DIR		=	$(SRCS_DIR)tools/

UTILS_DIR		=	$(SRCS_DIR)utils/

SRCS			=	$(SRCS_DIR)loop.c $(SRCS_DIR)minishell.c \
					$(BUILTIN_DIR)builtin.c $(BUILTIN_DIR)cd.c $(BUILTIN_DIR)echo.c $(BUILTIN_DIR)env.c $(BUILTIN_DIR)exit.c \
					$(BUILTIN_DIR)export.c $(BUILTIN_DIR)pwd.c $(BUILTIN_DIR)unset.c $(BUILTIN_DIR)utils.c \
					$(EXECUTOR_DIR)executor_utils.c $(EXECUTOR_DIR)executor_utils2.c $(EXECUTOR_DIR)executor_utils3.c $(EXECUTOR_DIR)executor.c \
					$(EXPANDER_DIR)expander_utils.c $(EXPANDER_DIR)expander.c \
					$(LEXER_DIR)lexer.c $(LEXER_DIR)syntax.c \
					$(PARSER_DIR)parser_utils.c $(PARSER_DIR)parser_utils2.c $(PARSER_DIR)parser.c \
					$(TOOLS_DIR)garbagecollector.c \
					$(UTILS_DIR)heredoc.c $(UTILS_DIR)signals.c $(UTILS_DIR)utils.c $(UTILS_DIR)utils2.c $(UTILS_DIR)utils3.c \

OBJS			=	$(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

CC				=	@cc
CFLAGS			=	$(INCLUDES)

INCLUDES		=	-I ./include

RM				=	@rm -rf

NAME			=	minishell

LIBFT_MAKE		=	@make -C ./libft
LIBFT			=	./libft/libft.a

RDFLAGS			=	-lreadline

COLOR_YELLOW	=	\033[0;33m
COLOR_GREEN		=	\033[0;32m
COLOR_RED		=	\033[0;31m
COLOR_END		=	\033[0m

all				:	$(NAME)

$(NAME)			:	$(LIBFT) $(OBJS)
					$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(RDFLAGS)
					@printf "$(COLOR_GREEN)\e[1mMINISHELL		$(COLOR_GREEN)\e[1mMinis$(COLOR_RED)\e[1m🔥HELL🔥$(COLOR_GREEN) \e[1mis \e[1mready\e[0m\n$(COLOR_END)"

$(OBJS_DIR)%.o	:	$(SRCS_DIR)%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) -c $< -o $@

clean			:
					$(RM) $(OBJS_DIR)
					$(LIBFT_MAKE) clean
					@printf "\e[1mOBJECT \e[1mFILES		$(COLOR_YELLOW)\e[1m🧹Cleaned🧹\e[0m\n$(COLOR_END)"

fclean			:	clean
					$(RM) $(NAME)
					$(LIBFT_MAKE) fclean
					@printf "\e[1mEXECUTABLE \e[1mFILES	$(COLOR_YELLOW)\e[1m🧹Cleaned🧹\e[0m\n$(COLOR_END)"

re				:	fclean all

$(LIBFT)		:
					$(LIBFT_MAKE) all

.PHONY			:	all clean fclean re
