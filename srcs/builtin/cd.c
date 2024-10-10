/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:27:16 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/10 17:01:49 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*home_path(t_env *home)
{
	char	*path;

	if (home)
	{
		path = check_malloc(ft_strdup(home->value));
		return (path);
	}
	else
		return (NULL);
}

static char	*old_pwd(t_command *cmd, t_env *oldpwd)
{
	char	*path;

	if (oldpwd)
	{
		path = check_malloc(ft_strdup(oldpwd->value));
		ft_putendl_fd(path, cmd->fd[1]);
		return (path);
	}
	return (NULL);
}

static char	*get_path(t_command *cmd, t_env *oldpwd, t_env *home)
{
	if (cmd->value[1])
	{
		if (cmd->value[1][0] != '-')
			return (check_malloc(ft_strdup(cmd->value[1])));
		else
			return (old_pwd(cmd, oldpwd));
	}
	else
		return (home_path(home));
}

int	cd(t_command *cmd, t_global *g)
{
	t_env	*env_pwd;
	char	*path;
	char	*wd;

	if (!check_flag(cmd))
		return (1);
	wd = check_malloc(getcwd(NULL, 0));
	env_pwd = sync_env(&g->env, "PWD", wd);
	if (!env_pwd)
	{
		g->error_no = 1;
		error_program(0, g->error_no);
	}
	path = get_path(cmd, env_finder("OLDPWD"), env_finder("HOME"));
	cd_sync(g, path, env_pwd->value, NULL);
	g->the_env = list_to_char(g->env);
	return (1);
}
