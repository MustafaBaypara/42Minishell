/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:15:36 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 14:35:52 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	exit_func(t_command *cmd, t_global *g)
{
	size_t	i;

	i = 0;
	while (cmd->value[i]) // komut değerlerini sayar
		i++;
	ft_putendl_fd("exit", cmd->fd[1]); 
	if (i > 1) // eğer komut değerleri 1'den büyükse
	{
		if (is_numeric(cmd->value[1]))  // eğer komut değeri sayısal ise
		{
			g->error_no = (ft_atoi(cmd->value[1]) % 256);
			if (i > 2) // eğer komut değerleri 2'den büyük
				return (g->error_no = 1,
					ft_putendl_fd("exit: too many arguments", 2), 1);
		}
		else // eğer komut değeri sayısal değilse
		{
			ft_putendl_fd("exit: numeric argument required", 2);
			g->error_no = 2;
		}
	}
	error_program(0, g->error_no); // exit atar
	return (1);
}
