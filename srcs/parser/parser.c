/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 16:48:08 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/12 16:48:08 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_command    *cmd_init(t_list token)
{
	t_command	*cmd;
	size_t		i;

	if (!token)
		return (NULL);
	cmd = check_malloc(ft_calloc(1, sizeof(t_command)));
	if (is_token(token))
		token = token.next;
		
}

void parser(t_global *g)
{
	t_command	cmd;

	cmd = cmd_init(g->token_list);
}