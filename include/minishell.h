/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:46:15 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/01 14:51:37 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "unistd.h"
# include "../srcs/libft/libft.h"
# include "shellutils.h"
# include "errorlib.h"

void			*add_list(t_list **list, void *garbage);
void			clear_garbage(t_list **lst);
void			loop(t_global *g);
t_list			*env_dup(char **env, t_global *g);
void			error_program(char *str, int err);
t_global		*_global(t_global *g);
void			*check_malloc(void *ptr);
char			**ft_split_first(const char *s, char c);
t_token_types	identifier(char *token);
int				is_white_space(char c);
char			*quote_clean(char *str, int s, int d);
t_env			*env_finder(char *str);
char			*dollar_sign(char *tmp, char *t_val, size_t *i, t_global *g);
int				check_hdoc(t_command *cmd);

// LEXER
void			lexer(t_global *g);

// PARSER
void			parser(t_global *g);
size_t			token_len(t_list *list);
size_t			word_end(const char *v, size_t start, int *d, int *s);
void			toggle_quote(char c, int *in_s, int *in_d);
int				rdr_position(t_command *cmds);

// EXPANDER
void			expander(t_global *g);
int				dollar(t_global	*g, char **value);
int				home(t_global *g, char **value);

// HEREDOC
int				heredocs(t_global *g, t_command *cmd);

// SIGNALS
void			catch_signal(int num);

#endif