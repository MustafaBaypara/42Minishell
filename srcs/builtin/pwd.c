/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:36:20 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 12:42:27 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	pwd(t_command *cmd, t_global *g)
{
	char	*pwd;

	if (!check_flag(cmd))
		return (1);
	pwd = getcwd(NULL, 0); // getcwd() ile çalışma dizinini alır
	if (pwd)
	{
		pwd = check_malloc(pwd);
		ft_putendl_fd(pwd, cmd->fd[1]);
	}
	else
		ft_putendl_fd(env_finder("PWD")->value, cmd->fd[1]); // yoksa PWD ortam değişkenini alır
	return (g->error_no = 0, 1);
}
