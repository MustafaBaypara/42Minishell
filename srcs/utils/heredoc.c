/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:50:22 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/27 14:52:08 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <cstdlib>

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

static int	loop_heredoc(t_global *g, int *fd, t_command *cmd, char *d)
{
	if (!d)
	{
		g->error_no = 12;
		close(fd[0]);
		close(fd[1]);
		error_program(ERROR_MALLOC, 1);
	}
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (g->error_no = 1, error_program(ERROR_FORK, 1), 0)
	
}

int	heredocs(t_global *g, t_command *cmd)
{
	int	fd[2];
	int	i;

	if (check_hdoc(cmd) == 0)
		return (1);
	signal_reciever(5);
	while (cmd)
	{
		i = -1;
		while (cmd->rds && cmd->rds[++i])
		{
			if (!ft_strncmp(cmd->rds[i], "<<", 2))
			{
				if (pipe(fd) == -1)
					return (error_program(ERROR_PIPE, 1), 0);
				i++;
				cmd->rds[i] = quote_clean(cmd->rds[i], 0, 0)
				if (loop_heredoc())
			}
		}
	}
}
