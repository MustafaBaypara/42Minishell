/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:15:36 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/08 17:27:39 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int  is_numeric(char *str)
{
    size_t  i;

    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (i);
}

int _exit(t_command *cmd, t_global *g)
{
    size_t  i;

    i = 0;
    while (cmd->value[i])
        i++;
    ft_putendl_fd("exit", cmd->fd[1]);
    if (i > 1)
    {
    }
}
