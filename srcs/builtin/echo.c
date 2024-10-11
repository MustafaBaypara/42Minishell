/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:46:39 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/11 18:17:50 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	counter(size_t *i, size_t *j, int *nl, t_command *cmd)
{
	(*i) = 1;
	(*nl) = 1;
	if (cmd->value[*i] && cmd->value[*i][0] == '-' && cmd->value[*i][1] == 'n')
	{
		while (cmd->value[*i] && cmd->value[*i][0] == '-')
		{
			(*j) = 1;
			while (cmd->value[*i][*j] == 'n')
				(*j)++;
			if (cmd->value[*i][*j] == '\0')
				(*nl) = 0;
			else
				break ;
			(*i)++;
		}
	}
}

int	echo(t_command *c, t_global *g)
{
	size_t	i;
	size_t	j;
	int		nline;

	counter(&i, &j, &nline, c);
	if (c->value[i])
		ft_putstr_fd(c->value[i++], c->fd[1]);
	while (c->value[i])
	{
		ft_putstr_fd(" ", c->fd[1]);
		ft_putstr_fd(c->value[i++], c->fd[1]);
	}
	if (nline)
		ft_putstr_fd("\n", c->fd[1]);
	return (g->error_no = 0, 1);
}
