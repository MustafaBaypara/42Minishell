/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:57:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/07 17:02:40 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_exit(t_global *g, int num)
{
	g->error_no = 12;
	if (num)
		error_program(0, g->error_no);
	return (0);
}

int	execute_builtin(t_command *cmd, int num,
	int (*f)(t_command *cmd, t_global *g))
{
	t_global	*g;

	g = _global(NULL);
	if (!(*f)(cmd, g))
		return (handle_exit(g, num));
	if (num)
		error_program(0, g->error_no);
	return (0);
}

int	builtin_check(t_command *cmd, int num)
{
	if (!ft_strncmp(cmd->value[0], "echo", ft_strlen("echo")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "cd", ft_strlen("cd")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "pwd", ft_strlen("pwd")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "env", ft_strlen("env")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "unset", ft_strlen("unset")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "export", ft_strlen("export")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "exit", ft_strlen("exit")))
		return (execute_builtin(cmd, num, echo));
	return (1);
}
