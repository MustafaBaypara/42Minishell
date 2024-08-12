/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellutils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:37:27 by mbaypara          #+#    #+#             */
/*   Updated: 2024/07/29 17:02:01 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELLUTILS_H
# define SHELLUTILS_H

# define INVALID_ARG 1

# include "stdio.h"

typedef struct s_global
{
	int		error_no;
	int		single_quotes;
	int		double_quotes;

	char	*tmp;
	char	*command_line;

	t_list	*token_list;
	t_list	*garbage_list;
	t_list	*env;
}	t_global;

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

#endif
