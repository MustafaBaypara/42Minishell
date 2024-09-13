/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellutils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:37:27 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/13 17:28:25 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELLUTILS_H
# define SHELLUTILS_H

# define INVALID_ARG 1

# include "stdio.h"

typedef enum e_token_types
{
	CMD,
	PIPE,
	RDR_IN,
	RDRD_IN,
	RDR_OUT,
	HEREDOC
}	t_token_types;

typedef struct s_command
{
	int		is_work;
	char	**value;
	char	**redirects;
	char	*cmdpath;
	int		fd[2];
	int		the_fd;
	int		p_type;
	int		pid;
}	t_command;

typedef struct s_global
{
	int		control;
	int		error_no;
	int		single_quotes;
	int		double_quotes;

	char	*tmp;
	char	*command_line;

	t_list	*token_list;
	t_list	*garbage_list;
	t_list	*env;
	t_list	*cmd_list;
}	t_global;

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

#endif
