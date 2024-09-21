/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:50:22 by mbaypara          #+#    #+#             */
/*   Updated: 2024/09/21 16:51:45 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	heredocs(t_global *g, t_command *cmd)
{
	int	fd[2];
	int	i;

	if (check_heredoc(cmd) == 0)
		return (1);
	signal_reciever(5);
	while (cmd)
	{
		
	}
}
