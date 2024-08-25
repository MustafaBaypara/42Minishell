/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:46:15 by mbaypara          #+#    #+#             */
/*   Updated: 2024/08/25 18:33:38 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include "../srcs/libft/libft.h"
# include "shellutils.h"
# include "errorlib.h"

void		*add_list(t_list **list, void *garbage);
void		clear_garbage(t_list **lst);
void		loop(t_global *g);
t_list		*env_dup(char **env, t_global *g);
void		error_program(char *str, int err);
t_global	*_global(t_global *g);
void		*check_malloc(void *ptr);
void		lexer(t_global *g);

#endif