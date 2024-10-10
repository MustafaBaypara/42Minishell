/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:36:20 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/10 16:41:24 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	pwd(t_command *cmd, t_global *g)
{
	char	*pwd;

	if (!check_flag(cmd))
		return (1);
	pwd = check_malloc(getcwd(NULL, 0));
	if (pwd)
		ft_putendl_fd(pwd, cmd->fd[1]);
	else
		return (0);
	return (g->error_no = 0, 1);
}
