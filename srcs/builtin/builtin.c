/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:57:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/07 17:52:10 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_flag(t_command *c)
{
	if (!c->value[1])
		return (1);
	if (c->value[1][0] == '-' && (ft_strlen(c->value[1]) > 1))
	{
		ft_putendl_fd("minishell: flag is not expected.", c->fd[1]);
		return (0);
	}
	return (1);
}

static int	execute_builtin(t_command *cmd, int num,
	int (*f)(t_command *cmd, t_global *g))
{
	t_global	*g;

	g = _global(NULL);
	if (!(*f)(cmd, g))
		return (error_program(0, 12), 1);
	if (num)
		error_program(0, g->error_no);
	else
		error_program(0, g->error_no);
	return (0);
}

int	builtin_check(t_command *cmd, int num)
{
	if (!cmd->value[0])
		return (1);
	if (!ft_strncmp(cmd->value[0], "echo", ft_strlen("echo")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "cd", ft_strlen("cd")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "pwd", ft_strlen("pwd")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "env", ft_strlen("env")))
		return (execute_builtin(cmd, num, env));
	else if (!ft_strncmp(cmd->value[0], "unset", ft_strlen("unset")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "export", ft_strlen("export")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "exit", ft_strlen("exit")))
		return (execute_builtin(cmd, num, echo));
	return (1);
}
