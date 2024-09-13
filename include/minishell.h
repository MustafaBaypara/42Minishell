/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:46:15 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/13 17:41:48 by mbaypara         ###   ########.fr       */
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
// LEXER
void			lexer(t_global *g);

// PARSER
void			parser(t_global *g, t_list *token, size_t i, t_command *cmd);
size_t			token_len(t_list *list);

#endif