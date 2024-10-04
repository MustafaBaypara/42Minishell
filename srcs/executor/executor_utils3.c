/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:52:38 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/04 13:56:15 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	remove_double(char **str)
{
	size_t	i;

	i = -1;
	while (str[++i])
		remove_from_list(&_global(NULL)->garbage_list, str[i]);
	remove_from_list(&_global(NULL)->garbage_list, str);
}

void	prepare_next(t_global *g)
{
	t_command	*cmd;
	t_command	*tmp;

	remove_double(g->path);
	remove_from_list(&g->garbage_list, g->command_line);
	remove_from_list(&g->garbage_list, g->tmp);
	clean_list(&g->token_list);
	g->command_line = NULL;
	g->tmp = NULL;
	g->token_list = NULL;
	cmd = g->cmd_list;
	while (cmd)
	{
		tmp = cmd->next;
		remove_double(cmd->value);
		remove_from_list(&g->garbage_list, cmd->rds);
		remove_from_list(&g->garbage_list, cmd->cmdpath);
		remove_from_list(&g->garbage_list, cmd);
		cmd = tmp;
	}
	g->cmd_list = NULL;
}
