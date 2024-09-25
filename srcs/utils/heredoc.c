/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:50:22 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/25 17:39:40 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_hdoc(t_command *cmd)
{
	int	i;
	int	j;

	j = 0;
	while (cmd)
	{
		i = 0;
		while (cmd->rds && cmd->rds[i])
			if (!ft_strncmp(cmd->rds[i++], "<<", 2))
				j++;
		cmd = cmd->next;
	}
	if (j == 0)
		return (0);
	return (1);
}

int	heredocs(t_global *g, t_command *cmd)
{
	int	fd[2];
	int	i;

	if (check_hdoc(cmd) == 0)
		return (1);
	//signal_reciever(5);
	while (cmd)
	{
		return (5);
	}
}
